#include "generator/emitter_restaurants.hpp"

#include "indexer/ftypes_matcher.hpp"
#include "generator/feature_builder.hpp"

using namespace feature;

namespace generator
{
EmitterRestaurants::EmitterRestaurants(std::vector<FeatureBuilder> & features)
  : m_features(features)
{
}

void EmitterRestaurants::Process(FeatureBuilder & fb)
{
  if (!ftypes::IsEatChecker::Instance()(fb.GetParams().m_types) || fb.GetParams().name.IsEmpty())
  {
    ++m_stats.m_unexpectedFeatures;
    return;
  }

  switch (fb.GetGeomType())
  {
  case feature::GeomType::Point: ++m_stats.m_restaurantsPoi; break;
  case feature::GeomType::Area: ++m_stats.m_restaurantsBuilding; break;
  default: ++m_stats.m_unexpectedFeatures;
  }
  m_features.emplace_back(fb);
}

void EmitterRestaurants::GetNames(std::vector<std::string> & names) const
{
  // We do not need to create any data file. See generator_tool.cpp and osm_source.cpp.
  names.clear();
}

bool EmitterRestaurants::Finish()
{
  LOG_SHORT(LINFO, ("Number of restaurants: POI:", m_stats.m_restaurantsPoi,
                    "BUILDING:", m_stats.m_restaurantsBuilding,
                    "TOTAL:", m_features.size(),
                    "INVALID:", m_stats.m_unexpectedFeatures));
  return true;
}
}  // namespace generator
