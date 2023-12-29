#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height)
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Initialize SDL_image
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
  {
    std::cerr << "SDL_image could not initialize.\n";
    std::cerr << "SDL_image_Error: " << IMG_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (sdl_window == nullptr)
  {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (sdl_renderer == nullptr)
  {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Load textures atlas
  atlas_texture = IMG_LoadTexture(sdl_renderer, "../resources/img/texture.png");
  if (atlas_texture == nullptr)
  {
    std::cerr << "Textures could not be loaded.\n";
    std::cerr << "IMG_Error: " << IMG_GetError() << "\n";
  }
}

Renderer::~Renderer()
{
  SDL_DestroyWindow(sdl_window);
  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyTexture(atlas_texture);
  SDL_Quit();
  IMG_Quit();
}

void Renderer::Render(Snake const snake, Food const &food)
{
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_Rect srcRect = {(food.GetLabel() + 5) * 75, 0, 75, 75};
  SDL_RenderCopy(sdl_renderer, atlas_texture, &srcRect, &block);

  // Render snake's body
  for (SDL_Point const &point : snake.body)
  {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    srcRect = {4 * 75, 0, 75, 75};
    SDL_RenderCopy(sdl_renderer, atlas_texture, &srcRect, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  int src_x = static_cast<int>(snake.direction) * 75;
  int src_y = 0;
  srcRect = {src_x, src_y, 75, 75};
  SDL_RenderCopy(sdl_renderer, atlas_texture, &srcRect, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps)
{
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
