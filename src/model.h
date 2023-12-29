#ifndef MODEL_H
#define MODEL_H

#include "snake.h"
#include <SDL2/SDL.h>
#include <string>

// Define some constants
const int kButtonX{310};
const int kButtonY{205};
const int kButtonW{180};
const int kButtonH{60};
const int kMessageX{200};
const int kMessageY{675};
const int kMessageW{400};
const int kMessageH{60};

// Forward declaration
class Snake;

class Food
{
public:
  enum class kType
  {
    kNormal,
    kSuper,
    kPoison
  };

  Food();
  Food(int x, int y, int type);
  bool AtCell(int cell_x, int cell_y);
  kType GetType() const { return type; }
  int GetLabel() const { return label; }

  int x;
  int y;

private:
  kType type;
  int label;
};

class Button
{
public:
  Button() { _area = {0, 0, 0, 0}; }
  Button(int x, int y, int w, int h, std::string text) : _text(text)
  {
    _area = {x, y, w, h};
    _ghost = {x + 5, y + 5, w, h};
  }

  bool IsMouseInside(int x, int y);
  void HandleClick(SDL_Event *e);
  bool IsClicked() { return _clicked; }

  // getter / setter
  int GetX() const { return _area.x; }
  int GetY() const { return _area.y; }
  int GetW() const { return _area.w; }
  int GetH() const { return _area.h; }
  SDL_Rect &GetArea() { return _area; }
  SDL_Rect &GetGhost() { return _ghost; }
  std::string GetText() { return _text; }
  void SetText(std::string text) { _text = text; }

private:
  std::string _text{};
  SDL_Rect _area;
  SDL_Rect _ghost;
  bool _clicked{false};
};

class TextBox
{
public:
  TextBox() {}
  TextBox(int x, int y, int w, int h, std::string text) : text(text) { box = {x, y, w, h}; }
  SDL_Rect box;
  std::string text;
};

class Window
{
public:
  virtual void HandleEvent(SDL_Event *e) = 0;
  Button quitButton;
};

class HomeWindow : public Window
{
public:
  HomeWindow();
  void HandleEvent(SDL_Event *e);

  Button playButton;
  Button logButton;
  TextBox messagebox;
  TextBox inputbox;
  bool inputting{false};
};

class PauseWindow : public Window
{
public:
  PauseWindow();
  void HandleEvent(SDL_Event *e);

  Button resumeButton;
  Button retryButton;
  Button homeButton;
};

class OverWindow : public Window
{
public:
  OverWindow();
  void HandleEvent(SDL_Event *e);

  Button retryButton;
  Button homeButton;
  TextBox messagebox;
};

#endif /* model.h */