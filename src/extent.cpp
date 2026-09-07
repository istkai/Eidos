#include "extent.hpp"

#include <limits>

namespace eidos {

ScaleResult try_scale_extent(Extent2D &extent, int factor) {
  if (extent.width < 0 || extent.height < 0) {
    return ScaleResult::NegativeDimension;
  }

  if (factor < 0) {
    return ScaleResult::NegativeFactor;
  }

  const int maximum{std::numeric_limits<int>::max()};

  if (factor != 0 &&
      (extent.width > maximum / factor || extent.height > maximum / factor)) {
    return ScaleResult::Overflow;
  }

  extent.width *= factor;
  extent.height *= factor;

  return ScaleResult::Success;
}

} // namespace eidos
