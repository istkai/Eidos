#include "draw_scene.hpp"

namespace eidos {

bool draw_scene(SDL_Renderer &renderer,
                std::span<const ColoredRectangle> rectangles) {
  if (!SDL_SetRenderDrawColor(&renderer, 20, 30, 50, 255) ||
      !SDL_RenderClear(&renderer) ||
      !SDL_SetRenderDrawBlendMode(&renderer, SDL_BLENDMODE_BLEND)) {
    return false;
  }

  for (const ColoredRectangle &rectangle : rectangles) {
    const SDL_Color &color{rectangle.color};

    if (!SDL_SetRenderDrawColor(&renderer, color.r, color.g, color.b,
                                color.a) ||
        !SDL_RenderFillRect(&renderer, &rectangle.bounds)) {
      return false;
    }
  }

  return SDL_RenderPresent(&renderer);
}

} // namespace eidos
