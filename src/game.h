#ifndef GAME_H
#define GAME_H

#include "controller.h"
#include "model.h"
#include "renderer.h"
#include <random>
#include <string>

#define LOG_FILE_PATH "../log.txt"

enum class kGameState
{
  kPlayingState,
  kPausingState,
  kHomeState,
  kOverState,
  kQuitState
};

class Game
{
public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  void HandleInput(HomeWindow &window, Renderer &renderer);
  void HandleInput(PauseWindow &window);
  void HandleInput(OverWindow &window);

  int GetScore() const;
  int GetHighScore() const;
  int GetSize() const;
  std::string GetUser() const;
  kGameState state;

private:
  HomeWindow homeWindow;
  PauseWindow pauseWindow;
  OverWindow overWindow;
  Snake snake;
  Food food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::uniform_int_distribution<int> random_label;

  int score{0};
  int high_score{0};
  bool logged{false};
  std::string usr{""};

  void PlaceFood();
  void Update();
  void Reset();
  void Log();
};

#endif