#pragma once

#include "generator/filter_interface.hpp"

namespace generator
{
class FilterPlanet : public FilterInterface
{
public:
  // FilterInterface overrides:
  std::shared_ptr<FilterInterface> Clone() const override;

  bool IsAccepted(OsmElement const & element) override;
  bool IsAccepted(feature::FeatureBuilder const & feature) override;
};
}  // namespace generator
