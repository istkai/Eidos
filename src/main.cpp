#include <cstdint>
#include <iostream>
#include <limits>

#include "extent.hpp"
#include "pixel_count.hpp"

int main() {
  const eidos::Extent2D resolution{
      .width = 1920,
      .height = 1080,
  };

  eidos::Extent2D scaled{resolution};

  if (eidos::try_scale_extent(scaled, 2) != eidos::ScaleResult::Success) {
    std::cerr << "Could not scale the extent." << '\n';

    return 1;
  };

  const std::int64_t original_pixel_count{
      eidos::calculate_pixel_count(resolution)};
  const std::int64_t scaled_pixel_count{eidos::calculate_pixel_count(scaled)};

  std::cout << "Eidos initializing..." << '\n';

  std::cout << "Original: " << resolution.width << " x " << resolution.height
            << '\n';

  std::cout << "Original pixel count: " << original_pixel_count << '\n';

  std::cout << "Scaled: " << scaled.width << " x " << scaled.height << '\n';

  std::cout << "Scaled pixel count: " << scaled_pixel_count << '\n';

  const eidos::Extent2D large_extent{70'000, 70'000};

  const std::int64_t large_pixel_count{
      eidos::calculate_pixel_count(large_extent)};

  std::cout << "Large pixel count: " << large_pixel_count << '\n';

  eidos::Extent2D oversized{1, std::numeric_limits<int>::max()};

  const bool success{eidos::try_scale_extent(oversized, 2) ==
                     eidos::ScaleResult::Success};

  std::cout << std::boolalpha << "Scaling succeeded: " << success << '\n';
  std::cout << "Extent afterwards: " << oversized.width << " x "
            << oversized.height << '\n';

  eidos::Extent2D scaled_by_zero{1920, 1080};
  eidos::Extent2D scaled_by_minus_one{scaled_by_zero};

  const bool zero_success{eidos::try_scale_extent(scaled_by_zero, 0) ==
                          eidos::ScaleResult::Success};
  const bool minus_one_success{
      eidos::try_scale_extent(scaled_by_minus_one, -1) ==
      eidos::ScaleResult::Success};

  std::cout << std::boolalpha
            << "Zero-factor scaling succeeded: " << zero_success << '\n';
  std::cout << "Extent afterwards: " << scaled_by_zero.width << " x "
            << scaled_by_zero.height << '\n';

  std::cout << std::boolalpha
            << "Minus-one-factor scaling succeeded: " << minus_one_success
            << '\n';
  std::cout << "Extent afterwards: " << scaled_by_minus_one.width << " x "
            << scaled_by_minus_one.height << '\n';

  eidos::Extent2D invalid{-1920, 1080};

  const eidos::ScaleResult result{eidos::try_scale_extent(invalid, -1)};

  switch (result) {
  case eidos::ScaleResult::Success:
    std::cout << "Scaling succeeded." << '\n';
    break;

  case eidos::ScaleResult::NegativeDimension:
    std::cout << "Dimensions must be non-negative." << '\n';
    break;

  case eidos::ScaleResult::NegativeFactor:
    std::cout << "The scale factor must be non-negative." << '\n';
    break;

  case eidos::ScaleResult::Overflow:
    std::cout << "The scaled dimensions would exceed int." << '\n';
    break;
  }

  return 0;
}
