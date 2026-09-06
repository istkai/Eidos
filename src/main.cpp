#include <iostream>

#include "pixel_count.hpp"

int main() {
  const int width{1920};
  const int height{1080};
  int pixel_count = calculate_pixel_count(width, height);

  std::cout << "Eidos initializing..." << '\n';
  std::cout << "Resolution: " << width << " x " << height << '\n';
  std::cout << "Pixel count: " << pixel_count << '\n';

  return 0;
}
