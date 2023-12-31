#include "controller.h"
#include "SDL2/SDL.h"
#include "model.h"
#include <iostream>

#include "game.h"

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const
{
  if (snake.direction != input && snake.direction != opposite || snake.size == 1)
    snake.direction = snake.poisoned ? opposite : input;
  return;
}

void Controller::HandleInput(kGameState &state, Snake &snake) const
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
    {
      state = kGameState::kQuitState;
      return;
    }
    else if (e.type == SDL_KEYDOWN)
    {
      switch (e.key.keysym.sym)
      {
      case SDLK_w:
      case SDLK_UP:
        ChangeDirection(snake, Snake::Direction::kUp,
                        Snake::Direction::kDown);
        break;

      case SDLK_s:
      case SDLK_DOWN:
        ChangeDirection(snake, Snake::Direction::kDown,
                        Snake::Direction::kUp);
        break;

      case SDLK_a:
      case SDLK_LEFT:
        ChangeDirection(snake, Snake::Direction::kLeft,
                        Snake::Direction::kRight);
        break;

      case SDLK_d:
      case SDLK_RIGHT:
        ChangeDirection(snake, Snake::Direction::kRight,
                        Snake::Direction::kLeft);
        break;

      case SDLK_p:
      case SDLK_ESCAPE:
        state = kGameState::kPausingState;
        break;
      }
    }
  }
}