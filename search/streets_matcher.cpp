#include "search/streets_matcher.hpp"
#include "search/features_filter.hpp"
#include "search/house_numbers_matcher.hpp"
#include "search/query_params.hpp"

#include "indexer/search_string_utils.hpp"

#include "base/logging.hpp"
#include "base/stl_helpers.hpp"

#include <algorithm>

using namespace std;

namespace search
{
namespace
{
bool LessByHashAndRange(StreetsMatcher::Prediction const & lhs,
                        StreetsMatcher::Prediction const & rhs)
{
  if (lhs.m_hash != rhs.m_hash)
    return lhs.m_hash < rhs.m_hash;

  if (lhs.GetNumTokens() != rhs.GetNumTokens())
    return lhs.GetNumTokens() > rhs.GetNumTokens();

  if (lhs.m_tokenRange.Begin() != rhs.m_tokenRange.Begin())
    return lhs.m_tokenRange.Begin() < rhs.m_tokenRange.Begin();

  if (lhs.m_prob != rhs.m_prob)
    return lhs.m_prob > rhs.m_prob;

  if (lhs.m_withMisprints != rhs.m_withMisprints)
    return rhs.m_withMisprints;

  return false;
}

bool EqualsByHashAndRange(StreetsMatcher::Prediction const & lhs,
                          StreetsMatcher::Prediction const & rhs)
{
  return lhs.GetNumTokens() == rhs.GetNumTokens() &&
         lhs.m_tokenRange.Begin() == rhs.m_tokenRange.Begin() && lhs.m_hash == rhs.m_hash;
}

bool LessByHashAndMisprints(StreetsMatcher::Prediction const & lhs,
                            StreetsMatcher::Prediction const & rhs)
{
  if (lhs.m_hash != rhs.m_hash)
    return lhs.m_hash < rhs.m_hash;

  if (lhs.m_withMisprints != rhs.m_withMisprints)
    return rhs.m_withMisprints;

  if (lhs.m_prob != rhs.m_prob)
    return lhs.m_prob > rhs.m_prob;

  if (lhs.GetNumTokens() != rhs.GetNumTokens())
    return lhs.GetNumTokens() > rhs.GetNumTokens();

  return lhs.m_tokenRange.Begin() < rhs.m_tokenRange.Begin();
}

bool EqualsByHashAndMisprints(StreetsMatcher::Prediction const & lhs,
                              StreetsMatcher::Prediction const & rhs)
{
  return lhs.m_withMisprints == rhs.m_withMisprints && lhs.m_hash == rhs.m_hash;
}

void FindStreets(BaseContext const & ctx, FeaturesFilter const & filter, QueryParams const & params,
                 size_t startToken, bool withMisprints,
                 vector<StreetsMatcher::Prediction> & predictions)
{
  // Here we try to match as many tokens as possible while
  // intersection is a non-empty bit vector of streets. Single
  // tokens that are synonyms to streets are ignored.  Moreover,
  // each time a token that looks like a beginning of a house number
  // is met, we try to use current intersection of tokens as a
  // street layer and try to match BUILDINGs or POIs.
  CBV streets(ctx.m_streets);

  CBV all;
  all.SetFull();

  size_t curToken = startToken;

  // This variable is used for prevention of duplicate calls to
  // CreateStreetsLayerAndMatchLowerLayers() with the same
  // arguments.
  size_t lastToken = startToken;

  // When true, no bit vectors were intersected with |streets| at all.
  bool emptyIntersection = true;

  // When true, |streets| is in the incomplete state and can't be
  // used for creation of street layers.
  bool incomplete = false;

  auto emit = [&]() {
    if (streets.IsEmpty() || emptyIntersection || incomplete || lastToken == curToken)
      return;

    CBV fs(streets);
    CBV fa(all);

    ASSERT(!fs.IsFull(), ());
    ASSERT(!fa.IsFull(), ());

    if (filter.NeedToFilter(fs))
      fs = filter.Filter(fs);

    if (fs.IsEmpty())
      return;

    if (filter.NeedToFilter(fa))
      fa = filter.Filter(fa).Union(fs);

    predictions.emplace_back();
    auto & prediction = predictions.back();

    prediction.m_tokenRange = TokenRange(startToken, curToken);

    ASSERT_NOT_EQUAL(fs.PopCount(), 0, ());
    ASSERT_LESS_OR_EQUAL(fs.PopCount(), fa.PopCount(), ());
    prediction.m_prob = static_cast<double>(fs.PopCount()) / static_cast<double>(fa.PopCount());

    prediction.m_features = move(fs);
    prediction.m_hash = prediction.m_features.Hash();
    prediction.m_withMisprints = withMisprints;
  };

  StreetTokensFilter streetsFilter(
      [&](strings::UniString const & /* token */, size_t tag) {
        auto buffer = streets.Intersect(ctx.m_features[tag].m_features);
        if (tag < curToken)
        {
          // This is the case for delayed
          // street synonym.  Therefore,
          // |streets| is temporarily in the
          // incomplete state.
          streets = buffer;
          all = all.Intersect(ctx.m_features[tag].m_features);
          emptyIntersection = false;

          incomplete = true;
          return;
        }
        ASSERT_EQUAL(tag, curToken, ());

        // |streets| will become empty after
        // the intersection. Therefore we need
        // to create streets layer right now.
        if (buffer.IsEmpty())
          emit();

        streets = buffer;
        all = all.Intersect(ctx.m_features[tag].m_features);
        emptyIntersection = false;
        incomplete = false;
      },
      withMisprints);

  for (; curToken < ctx.m_numTokens && !ctx.IsTokenUsed(curToken) && !streets.IsEmpty(); ++curToken)
  {
    auto const & token = params.GetToken(curToken).GetOriginal();
    bool const isPrefix = params.IsPrefixToken(curToken);

    if (house_numbers::LooksLikeHouseNumber(token, isPrefix))
      emit();

    streetsFilter.Put(token, isPrefix, curToken);
  }
  emit();
}
}  // namespace

// static
void StreetsMatcher::Go(BaseContext const & ctx, FeaturesFilter const & filter,
                        QueryParams const & params, vector<Prediction> & predictions)
{
  size_t const kMaxNumOfImprobablePredictions = 3;
  double const kTailProbability = 0.05;

  predictions.clear();
  FindStreets(ctx, filter, params, predictions);

  if (predictions.empty())
    return;

  // Remove predictions with the same m_hash (features) and token range.
  base::SortUnique(predictions, &LessByHashAndRange, &EqualsByHashAndRange);

  // Leave the most probable and longest prediction for predictions with the same m_hash (features)
  // and m_withMisprints.
  // We will still distinguish parses with the same m_hash (features) but different range and
  // m_withMisprints. For example, for "Paramount dive" we will have two parses:
  //
  // STREET       UNUSED (can be matched to poi later)
  // Paramount   dive
  //
  // STREET       STREET ("drive" with misprints)
  // Paramount    dive
  //
  // The parses will have the same features and hash but we need both of them.
  base::SortUnique(predictions, &LessByHashAndMisprints, &EqualsByHashAndMisprints);

  sort(predictions.rbegin(), predictions.rend(), base::LessBy(&Prediction::m_prob));
  while (predictions.size() > kMaxNumOfImprobablePredictions &&
         predictions.back().m_prob < kTailProbability)
  {
    predictions.pop_back();
  }
}

// static
void StreetsMatcher::FindStreets(BaseContext const & ctx, FeaturesFilter const & filter,
                                 QueryParams const & params, vector<Prediction> & predictions)
{
  for (size_t startToken = 0; startToken < ctx.m_numTokens; ++startToken)
  {
    if (ctx.IsTokenUsed(startToken))
      continue;

    ::search::FindStreets(ctx, filter, params, startToken, false /* withMisprints */, predictions);
    ::search::FindStreets(ctx, filter, params, startToken, true /* withMisprints */, predictions);
  }
}
}  // namespace search
