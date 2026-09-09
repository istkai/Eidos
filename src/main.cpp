#include <array>
#include <iomanip>
#include <iostream>
#include <memory>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "draw_scene.hpp"
#include "extent.hpp"

/// ANSI code - moves the cursor one unit up
#define CURSOR_MOVE_UP "\033[1A"
/// ANSI code - moves the cursor one unit down
#define CURSOR_MOVE_DOWN "\033[1B"
/// ANSI code - moves the cursor to the end of the line by moving 999 units to
/// the right
#define CURSOR_MOVE_ENDL "\033[999C"

struct SdlShutdown {
  SdlShutdown() = default;

  SdlShutdown(const SdlShutdown &) = delete;
  SdlShutdown &operator=(const SdlShutdown &) = delete;

  ~SdlShutdown() {
    std::cout << "Shutting down SDL..." << '\n';

    SDL_Quit();
  }
};

struct WindowDeleter {
  void operator()(SDL_Window *window) const {
    std::cout << "Closing window..." << '\n';
    SDL_DestroyWindow(window);
  }
};

struct RendererDeleter {
  void operator()(SDL_Renderer *renderer) const {
    std::cout << "Dropping renderer..." << '\n';
    SDL_DestroyRenderer(renderer);
  }
};

using Eidos_Window = std::unique_ptr<SDL_Window, WindowDeleter>;
using Eidos_Renderer = std::unique_ptr<SDL_Renderer, RendererDeleter>;
using Eidos_VSync = std::unique_ptr<int>;

int main(int, char *[]) {
  if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
    std::cerr << "SDL initialization failed: " << SDL_GetError() << '\n';

    SDL_Quit();

    return 1;
  }

  SdlShutdown sdl_shutdown{};

  Eidos_Window window{SDL_CreateWindow("Eidos", 960, 540, 0)};

  if (window == nullptr) {
    std::cerr << "Window creation failed: " << SDL_GetError() << '\n';

    return 1;
  }

  Eidos_Renderer renderer{SDL_CreateRenderer(window.get(), nullptr)};

  if (renderer == nullptr) {
    std::cerr << "Renderer creation failed: " << SDL_GetError() << '\n';

    return 1;
  }

  Eidos_VSync vsync{std::make_unique<int>(SDL_RENDERER_VSYNC_DISABLED)};

  if (!SDL_SetRenderVSync(renderer.get(), *vsync)) {
    std::cerr << "Enabling VSync failed: " << SDL_GetError() << '\n';

    return 1;
  }

  const char *video_driver{SDL_GetCurrentVideoDriver()};
  const char *renderer_name{SDL_GetRendererName(renderer.get())};

  if (video_driver != nullptr) {
    std::cout << "Video driver: " << video_driver << '\n';
  }

  if (renderer_name != nullptr) {
    std::cout << "Renderer: " << renderer_name << CURSOR_MOVE_DOWN << '\n'
              << '\n';
  }

  SDL_FRect rectangle{
      .x = 100.0f,
      .y = 80.0f,
      .w = 240.0f,
      .h = 140.0f,
  };

  const float horizontal_speed{750.0f};
  float horizontal_velocity{horizontal_speed};

  Uint64 previous_ticks{SDL_GetTicksNS()};

  Uint64 report_start_ticks{previous_ticks};
  int frames_since_report{0};

  bool running{true};
  bool paused{false};

  while (running) {
    SDL_Event event{};

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      } else if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
        switch (event.key.key) {
        case SDLK_SPACE:
          paused = !paused;
          break;

        case SDLK_R:
          horizontal_velocity = -horizontal_velocity;
          break;

        case SDLK_V:
          switch (*vsync) {
          case SDL_RENDERER_VSYNC_DISABLED:
            if (!SDL_SetRenderVSync(renderer.get(),
                                    SDL_RENDERER_VSYNC_ADAPTIVE)) {
              std::cerr << "Setting VSync failed: " << SDL_GetError() << '\n';
            } else {
              *vsync = SDL_RENDERER_VSYNC_ADAPTIVE;
            }

            break;

          case SDL_RENDERER_VSYNC_ADAPTIVE:
            if (!SDL_SetRenderVSync(renderer.get(),
                                    SDL_RENDERER_VSYNC_DISABLED)) {
              std::cerr << "Setting VSync failed: " << SDL_GetError() << '\n';
            } else {
              *vsync = SDL_RENDERER_VSYNC_DISABLED;
            }

            break;

          default:
            break;
          }

          break;

        case SDLK_ESCAPE:
        case SDLK_Q:
          running = false;
          break;

        default:
          break;
        }
      }
    }

    if (!running) {
      std::cout << '\n' << '\n';

      break;
    }

    const Uint64 current_ticks{SDL_GetTicksNS()};

    const float delta_seconds{
        static_cast<float>(current_ticks - previous_ticks) / 1'000'000'000.0f};

    previous_ticks = current_ticks;

    eidos::Extent2D output_size{};

    if (!SDL_GetRenderOutputSize(renderer.get(), &output_size.width,
                                 &output_size.height)) {
      std::cerr << "Reading output size failed: " << SDL_GetError() << '\n';

      return 1;
    }

    const float maximum_x{static_cast<float>(output_size.width) - rectangle.w};

    if (!paused) {
      if (maximum_x <= 0.0f) {
        rectangle.x = 0.0f;
        horizontal_velocity = horizontal_speed;
      } else {
        rectangle.x += horizontal_velocity * delta_seconds;

        while (rectangle.x < 0.0f || rectangle.x > maximum_x) {
          if (rectangle.x > maximum_x) {
            const float overshoot{rectangle.x - maximum_x};

            rectangle.x = maximum_x - overshoot;
            horizontal_velocity = -horizontal_speed;
          } else {
            rectangle.x = -rectangle.x;
            horizontal_velocity = horizontal_speed;
          }
        }

        if (rectangle.x == 0.0f) {
          horizontal_velocity = horizontal_speed;
        } else if (rectangle.x == maximum_x) {
          horizontal_velocity = -horizontal_speed;
        }
      }
    }

    const std::array<eidos::ColoredRectangle, 3> scene_rectangles{
        // Green Rectangle
        eidos::ColoredRectangle{
            .bounds =
                {
                    .x = 100.0f,
                    .y = 150.0f,
                    .w = 160.0f,
                    .h = 80.0f,
                },
            .color = {.r = 70, .g = 200, .b = 130, .a = 128},
        },

        // Orange Rectangle
        eidos::ColoredRectangle{
            .bounds = rectangle,
            .color = {.r = 240, .g = 150, .b = 50, .a = 255},
        },

        // Purple Rectangle
        eidos::ColoredRectangle{
            .bounds =
                {
                    .x = 500.0f,
                    .y = 300.0f,
                    .w = 80.0f,
                    .h = 160.0f,
                },
            .color = {.r = 170, .g = 110, .b = 230, .a = 255},
        },
    };

    if (!eidos::draw_scene(*renderer, scene_rectangles)) {
      std::cerr << "Drawing failed: " << SDL_GetError() << '\n';

      return 1;
    }

    ++frames_since_report;

    const Uint64 report_ticks{SDL_GetTicksNS()};

    const double report_seconds{
        static_cast<double>(report_ticks - report_start_ticks) /
        1'000'000'000.0};

    if (report_seconds >= 1.0) {
      const double frames_per_second{frames_since_report / report_seconds};

      const double average_frame_ms{report_seconds * 1000.0 /
                                    frames_since_report};

      const std::string vsync_state{
          *vsync == SDL_RENDERER_VSYNC_DISABLED ? "disabled" : "enabled "};

      std::cout << std::fixed << std::setprecision(0) << CURSOR_MOVE_UP << '\r'
                << "FPS: " << std::setw(4) << frames_per_second
                << " | Average frametime: " << std::setprecision(1)
                << std::setw(5) << average_frame_ms << " ms" << std::endl;

      std::cout << "VSync " << vsync_state << std::flush;

      frames_since_report = 0;
      report_start_ticks = report_ticks;
    }
  }

  return 0;
}
