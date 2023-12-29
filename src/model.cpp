#include "model.h"

/* Implementation of class Food */
Food::Food() : x(0), y(0), label(0), type(kType::kNormal) {}
Food::Food(int x, int y, int label) : x(x), y(y), label(label)
{
    if (label >= 0 && label <= 8)
    {
        type = Food::kType::kNormal;
    }
    else if (label == 9)
    {
        type = Food::kType::kSuper;
    }
    else
    {
        type = Food::kType::kPoison;
    }
}

bool Food::AtCell(int cell_x, int cell_y)
{
    return x == cell_x && y == cell_y;
}

/* Implementation of class Button */
bool Button::IsMouseInside(int x, int y)
{
    return (x >= _area.x && x <= _area.x + _area.w && y >= _area.y && y <= _area.y + _area.h);
}

void Button::HandleClick(SDL_Event *e)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    _clicked = IsMouseInside(x, y);
}

/* Implementation of class HomeWindow*/
HomeWindow::HomeWindow()
{
    playButton = {kButtonX, kButtonY + 110, kButtonW, kButtonH, "PLAY"};
    logButton = {kButtonX, kButtonY + 220, kButtonW, kButtonH, "LOG"};
    quitButton = {kButtonX, kButtonY + 330, kButtonW, kButtonH, "QUIT"};
    inputbox = {kMessageX, kButtonY, kMessageW, kMessageH, ""};
    messagebox = {kMessageX, kMessageY, kMessageW, kMessageH, "What should I call you?"};
}

void HomeWindow::HandleEvent(SDL_Event *e)
{
    playButton.HandleClick(e);
    logButton.HandleClick(e);
    quitButton.HandleClick(e);
}

/* Implementation of class PauseWindow*/
PauseWindow::PauseWindow()
{
    resumeButton = {kButtonX, kButtonY + 0, kButtonW, kButtonH, "RESUME"};
    retryButton = {kButtonX, kButtonY + 110, kButtonW, kButtonH, "NEW GAME"};
    homeButton = {kButtonX, kButtonY + 220, kButtonW, kButtonH, "HOME"};
    quitButton = {kButtonX, kButtonY + 330, kButtonW, kButtonH, "QUIT"};
}

void PauseWindow::HandleEvent(SDL_Event *e)
{
    resumeButton.HandleClick(e);
    retryButton.HandleClick(e);
    homeButton.HandleClick(e);
    quitButton.HandleClick(e);
}

/* Implementation of class OverWindow*/
OverWindow::OverWindow()
{
    retryButton = {kButtonX, kButtonY + 110, kButtonW, kButtonH, "NEW GAME"};
    homeButton = {kButtonX, kButtonY + 220, kButtonW, kButtonH, "HOME"};
    quitButton = {kButtonX, kButtonY + 330, kButtonW, kButtonH, "QUIT"};
    messagebox = {kMessageX, kMessageY, kMessageW, kMessageH, ""};
}

void OverWindow::HandleEvent(SDL_Event *e)
{
    retryButton.HandleClick(e);
    homeButton.HandleClick(e);
    quitButton.HandleClick(e);
}
