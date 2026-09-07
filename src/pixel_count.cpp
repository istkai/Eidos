#include "pixel_count.hpp"

#include <cstdint>

namespace eidos {

std::int64_t calculate_pixel_count(Extent2D extent) {
  return static_cast<std::int64_t>(extent.width) * extent.height;
}

} // namespace eidos
