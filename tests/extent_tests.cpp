#include <cassert>
#include <iostream>
#include <limits>

#include "extent.hpp"

void check_scale(eidos::Extent2D input, int factor,
                 eidos::ScaleResult expected_result,
                 eidos::Extent2D expected_extent) {
  const eidos::ScaleResult result{eidos::try_scale_extent(input, factor)};

  assert(result == expected_result);
  assert(input.width == expected_extent.width);
  assert(input.height == expected_extent.height);
}

int main() {
  const int maximum{std::numeric_limits<int>::max()};

  check_scale({1920, 1080}, 2, eidos::ScaleResult::Success, {3840, 2160});
  check_scale({1920, 1080}, 0, eidos::ScaleResult::Success, {0, 0});
  check_scale({1920, 1080}, -1, eidos::ScaleResult::NegativeFactor,
              {1920, 1080});
  check_scale({-1920, 1080}, 2, eidos::ScaleResult::NegativeDimension,
              {-1920, 1080});
  check_scale({1, maximum}, 2, eidos::ScaleResult::Overflow, {1, maximum});
  check_scale({maximum, 1}, 2, eidos::ScaleResult::Overflow, {maximum, 1});
  check_scale({-1920, 1080}, -1, eidos::ScaleResult::NegativeDimension,
              {-1920, 1080});

  std::cout << "All extent tests passed." << '\n';

  return 0;
}
