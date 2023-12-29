#ifndef RENDERER_H
#define RENDERER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "model.h"
#include <memory>
#include <string>

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
  std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> sdl_window{nullptr, SDL_DestroyWindow};
  std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> sdl_renderer{nullptr, SDL_DestroyRenderer};
  std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)> atlas_texture{nullptr, SDL_DestroyTexture};
  std::unique_ptr<TTF_Font, void (*)(TTF_Font *)> font{nullptr, TTF_CloseFont};

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif /* renderer.h */