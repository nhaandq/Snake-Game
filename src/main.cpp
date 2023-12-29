#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <iostream>
#include <memory>

int main()
{
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{800};
  constexpr std::size_t kScreenHeight{800};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  auto renderer = std::make_unique<Renderer>(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, std::move(renderer), kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "User: " << game.GetUser() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "High Score: " << game.GetHighScore() << "\n";
  return 0;
}