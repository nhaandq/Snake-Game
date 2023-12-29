#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"

enum class kGameState;

class Controller
{
public:
    void HandleInput(kGameState &state, Snake &snake) const;

private:
    void ChangeDirection(Snake &snake, Snake::Direction input,
                         Snake::Direction opposite) const;
};

#endif /* controller.h */