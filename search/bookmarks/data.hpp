#pragma once

#include "indexer/search_string_utils.hpp"

#include "kml/types.hpp"

#include "coding/string_utf8_multilang.hpp"

#include "base/string_utils.hpp"

#include <string>

namespace search
{
namespace bookmarks
{
// TODO (@m, @y): add more features for a bookmark here, i.e. address, center.
struct Data
{
  Data() = default;

  Data(kml::BookmarkData const & bookmarkData)
    : m_name(kml::GetDefaultStr(bookmarkData.m_name))
    , m_customName(kml::GetDefaultStr(bookmarkData.m_customName))
    , m_description(kml::GetDefaultStr(bookmarkData.m_description))
  {
  }

  template <typename Fn>
  void ForEachNameToken(Fn && fn) const
  {
    auto withDefaultLang = [&](strings::UniString const & token) {
      fn(StringUtf8Multilang::kDefaultCode, token);
    };

    ForEachNormalizedToken(m_name, withDefaultLang);
    ForEachNormalizedToken(m_customName, withDefaultLang);
  }

  template <typename Fn>
  void ForEachDescriptionToken(Fn && fn) const
  {
    auto withDefaultLang = [&](strings::UniString const & token) {
      fn(StringUtf8Multilang::kDefaultCode, token);
    };

    ForEachNormalizedToken(m_description, withDefaultLang);
  }

  std::string m_name;
  std::string m_customName;
  std::string m_description;
};

std::string DebugPrint(Data const & data);
}  // namespace bookmarks
}  // namespace search
