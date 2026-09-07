#ifndef EIDOS_PIXEL_COUNT_HPP
#define EIDOS_PIXEL_COUNT_HPP

#include <cstdint>

#include "extent.hpp"

namespace eidos {

/// Calculates and returns the total pixel count of a width/height pair.
/// Requires non-negative dimensions whose product fits in int.
std::int64_t calculate_pixel_count(Extent2D extent);

} // namespace eidos

#endif
