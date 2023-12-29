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

  // Initialize SDL_ttf
  if (TTF_Init() == -1)
  {
    std::cerr << "SDL_TTF could not initialize.\n";
    std::cerr << "SDL_TTF_Error: " << TTF_GetError() << "\n";
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
  atlas_texture = IMG_LoadTexture(sdl_renderer, TEXTURE_FILE_PATH);
  if (atlas_texture == nullptr)
  {
    std::cerr << "Textures could not be loaded.\n";
    std::cerr << "IMG_Error: " << IMG_GetError() << "\n";
  }

  // Load default font
  font = TTF_OpenFont(FONT_FILE_PATH, 20);
  if (atlas_texture == nullptr)
  {
    std::cerr << "Font could not be loaded.\n";
    std::cerr << "TTF_Error: " << TTF_GetError() << "\n";
  }
}

Renderer::~Renderer()
{
  SDL_DestroyWindow(sdl_window);
  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyTexture(atlas_texture);
  TTF_CloseFont(font);
  SDL_Quit();
  IMG_Quit();
  TTF_Quit();
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

// Render text
void Renderer::Render(std::string text, SDL_Rect area, SDL_Color color, TTF_Font *font, float const scale)
{
  font = (!font) ? this->font : font;
  SDL_Surface *text_surface = TTF_RenderText_Solid(font, text.c_str(), color);                // Render text to surface
  SDL_Texture *text_texture = SDL_CreateTextureFromSurface(sdl_renderer, text_surface); // Create texture from surface
  SDL_FreeSurface(text_surface);                                                              // Free surface

  // Calculate text actual area
  int w, h;
  TTF_SizeText(font, text.c_str(), &w, &h);
  w = static_cast<int>(w * scale);
  h = static_cast<int>(h * scale);
  SDL_Rect text_area = {area.x + (area.w - w) / 2, area.y + (area.h - h) / 2, w, h};

  // Copy texture to the renderer and free texture
  SDL_RenderCopy(sdl_renderer, text_texture, NULL, &text_area);
  SDL_DestroyTexture(text_texture);
}

// Render button
void Renderer::Render(Button button)
{
  // Set ghost color
  SDL_SetRenderDrawColor(sdl_renderer, 0x4D, 0x4C, 0x7D, 0x80);
  SDL_RenderFillRect(sdl_renderer, &button.GetGhost());

  // Set fill color
  SDL_SetRenderDrawColor(sdl_renderer, 0x80, 0x80, 0x80, 0xFF);
  SDL_RenderFillRect(sdl_renderer, &button.GetArea());

  // Render text on button
  SDL_Color color = {0xFF, 0xDE, 0x00, 0xFF};
  Render(button.GetText(), button.GetArea(), color, nullptr, 1);
}

// Render text box
void Renderer::Render(TextBox textbox)
{
  // Set box background color
  SDL_SetRenderDrawColor(sdl_renderer, 0x3B, 0x4C, 0xCA, 0xFF);
  SDL_RenderFillRect(sdl_renderer, &textbox.box);

  // Render text
  SDL_Color text_color = {0, 0, 0, 255};
  Render(textbox.text, textbox.box, text_color, nullptr, 1);

  // Update renderer
  SDL_RenderPresent(sdl_renderer);
}

// Render home screen
void Renderer::Render(HomeWindow window)
{
  std::string title{"Snake Game"};
  SDL_SetWindowTitle(sdl_window, title.c_str());

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x3B, 0x4C, 0xCA, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render text on the screen
  SDL_Color color = {0xFF, 0xDE, 0x00, 0xFF};
  SDL_Rect textRect = {50, 100, 700, 100}; // Create text rectangle
  std::string text = "SNaKe Go";
  Render(text, textRect, color, nullptr, 5);

  // Check if user input to render
  if (window.inputbox.text != "")
    Render(window.inputbox);

  // Render buttons on the screen
  Render(window.playButton);
  Render(window.logButton);
  Render(window.quitButton);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

// Render Pausing Window
void Renderer::Render(PauseWindow window)
{
  // Render buttons on the screen
  Render(window.resumeButton);
  Render(window.retryButton);
  Render(window.homeButton);
  Render(window.quitButton);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

// Render game over window
void Renderer::Render(OverWindow window, int const score)
{
  // Set window title
  std::string title{"Game Over - Score: " + std::to_string(score)};
  SDL_SetWindowTitle(sdl_window, title.c_str());

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x3B, 0x4C, 0xCA, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render text on the screen
  SDL_Color color = {0xFF, 0xDE, 0x00, 0xFF};
  SDL_Rect textRect = {80, 120, 640, 135}; // Create text rectangle
  Render("GAME OVER", textRect, color, nullptr, 4);

  // Render buttons on the screen
  Render(window.retryButton);
  Render(window.homeButton);
  Render(window.quitButton);

  // Render over message
  Render(window.messagebox);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps)
{
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
