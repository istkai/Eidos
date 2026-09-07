#ifndef EIDOS_EXTENT_HPP
#define EIDOS_EXTENT_HPP

namespace eidos {

/// A 2-dimensional size measured in pixels (width, height).
struct Extent2D {
  int width{};  // width
  int height{}; // height
};

enum class ScaleResult {
  Success,
  NegativeDimension,
  NegativeFactor,
  Overflow,
};

// Scales a 2D Extent by a provided factor if both the Extent's dimensions and
// the factor are non-negative and both resulting dimensions fit in int. Returns
// false without modifying the Extent if any of those conditions fail.
ScaleResult try_scale_extent(Extent2D &extent, int factor);

} // namespace eidos

#endif
