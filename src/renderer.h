#ifndef RENDERER_H
#define RENDERER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "model.h"

#define FONT_FILE_PATH "../resources/font/PIXEL.ttf"
#define TEXTURE_FILE_PATH "../resources/img/texture.png"

class Renderer
{
public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const snake, Food const &food);
  void Render(std::string text, SDL_Rect area, SDL_Color color, TTF_Font *font, float const scale);
  void Render(Button button);
  void Render(TextBox textbox);
  void Render(HomeWindow window);
  void Render(PauseWindow window);
  void Render(OverWindow window, int score);
  void UpdateWindowTitle(int score, int fps);

private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  SDL_Texture *atlas_texture;
  TTF_Font *font;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif