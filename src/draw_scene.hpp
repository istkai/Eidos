#ifndef EIDOS_DRAW_SCENE_HPP
#define EIDOS_DRAW_SCENE_HPP

#include <span>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

namespace eidos {

/// A bounded Rectangle that also stores its own color values as RGBA
struct ColoredRectangle {
  SDL_FRect bounds{};
  SDL_Color color{255, 255, 255, 255};
};

// Draws and presents one frame.
// Returns false if any drawing or presentation operation fails.
bool draw_scene(SDL_Renderer &renderer,
                std::span<const ColoredRectangle> rectangles);

} // namespace eidos

#endif
