#include "game.h"
#include "SDL2/SDL.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : state(kGameState::kHomeState),
      snake(grid_width, grid_height), food(),
      homeWindow(), pauseWindow(), overWindow(),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      random_label(0, 11)
{
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running)
  {
    switch (state)
    {
    case kGameState::kHomeState:
      frame_count = 0;
      renderer.Render(homeWindow);
      while (state == kGameState::kHomeState)
      {
        HandleInput(homeWindow, renderer);
      }
      break;

    case kGameState::kPlayingState:
      // Handle Game Loop if game state is still playing
      while (state == kGameState::kPlayingState)
      {
        frame_start = SDL_GetTicks();

        // Input, Update, Render - the main game loop.
        controller.HandleInput(state, snake);
        Update();
        renderer.Render(snake, food);

        frame_end = SDL_GetTicks();

        // Keep track of how long each loop through the input/update/render cycle takes
        frame_count++;
        frame_duration = frame_end - frame_start;

        // After every second, update the window title.
        if (frame_end - title_timestamp >= 1000)
        {
          renderer.UpdateWindowTitle(score, frame_count);
          frame_count = 0;
          title_timestamp = frame_end;
        }

        // If the time for this frame is too small (i.e. frame_duration is
        // smaller than the target ms_per_frame), delay the loop to
        // achieve the correct frame rate.
        if (frame_duration < target_frame_duration)
        {
          SDL_Delay(target_frame_duration - frame_duration);
        }
      }
      break;

    case kGameState::kPausingState:
      frame_count = 0;
      renderer.Render(pauseWindow);
      while (state == kGameState::kPausingState)
      {
        HandleInput(pauseWindow);
      }
      break;

    case kGameState::kOverState:
      frame_count = 0;
      renderer.Render(overWindow, score);
      while (state == kGameState::kOverState)
      {
        HandleInput(overWindow);
      }
      break;

    case kGameState::kQuitState:
      running = false;
      break;
    }
  }
}

// Handle click and input event on home screen
void Game::HandleInput(HomeWindow &window, Renderer &renderer)
{
  bool first_input = true;
  bool render_text = false;
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
    {
      state = kGameState::kQuitState;
      return;
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
      window.HandleEvent(&e);
      if (window.playButton.IsClicked())
      {
        state = kGameState::kPlayingState;
        return;
      }
      else if (window.logButton.IsClicked())
      {
        window.inputting = true;
        renderer.Render(window.messagebox);
      }
      else if (window.quitButton.IsClicked())
      {
        state = kGameState::kQuitState;
        return;
      }
    }
    else if (window.inputting)
    {
      // Special key input
      if (e.type == SDL_KEYDOWN)
      {
        // Handle backspace
        if (e.key.keysym.sym == SDLK_BACKSPACE && usr.length() > 0)
        {
          // Delete last character
          usr.pop_back();
          render_text = true;
        }
        // Handle copy
        else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
        {
          SDL_SetClipboardText(usr.c_str());
        }
        // Handle paste
        else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
        {
          // Copy text from temporary buffer
          char *temp_text = SDL_GetClipboardText();
          usr = temp_text;
          // Limit user name at 8 characters
          if (usr.length() > 8)
          {
            usr.erase(8);
          }

          SDL_free(temp_text);
          render_text = true;
        }
        // Handle escape
        else if (e.key.keysym.sym == SDLK_ESCAPE)
        {
          // Cancel current import and set game to not be logged
          window.inputting = false;
          window.inputbox.text = "";
          usr = "";
          window.logButton.SetText("LOG");
          renderer.Render(window);
        }
        // Handle enter
        else if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER)
        {
          // Finish current import and set game to be logged or not base on imported user name
          window.inputting = false;
          if (usr != "")
          {
            logged = true;
            window.logButton.SetText("LOGGED");
          }
          else
          {
            window.inputbox.text = "";
            logged = false;
            window.logButton.SetText("LOG");
          }
          renderer.Render(window);
        }
      }
      // Special text input event
      else if (e.type == SDL_TEXTINPUT)
      {
        // Not copy or pasting
        if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
        {
          // Append character
          if (usr.length() <= 8)
          {
            usr += e.text.text;
            render_text = true;
          }
        }
      }
    }
  }
  // Rerender text if needed
  if (render_text)
  {
    window.inputbox.text = "With ";
    // Text is not empty
    if (usr != "")
    {
      // Render new text
      window.inputbox.text += usr;
      renderer.Render(window.inputbox);
    }
    // Text is empty
    else
    {
      renderer.Render(window.inputbox);
    }
  }
}

// Handle button click event on pause screen
void Game::HandleInput(PauseWindow &window)
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
    {
      state = kGameState::kQuitState;
      return;
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
      pauseWindow.HandleEvent(&e);
      if (pauseWindow.resumeButton.IsClicked())
      {
        state = kGameState::kPlayingState;
        return;
      }
      else if (pauseWindow.retryButton.IsClicked())
      {
        Reset();
        state = kGameState::kPlayingState;
        return;
      }
      else if (pauseWindow.homeButton.IsClicked())
      {
        Reset();
        state = kGameState::kHomeState;
        return;
      }
      else if (pauseWindow.quitButton.IsClicked())
      {
        state = kGameState::kQuitState;
        return;
      }
      break;
    }
  }
}

// Handle button click event on over screen
void Game::HandleInput(OverWindow &window)
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
    {
      state = kGameState::kQuitState;
      return;
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
      overWindow.HandleEvent(&e);
      if (overWindow.retryButton.IsClicked())
      {
        Reset();
        state = kGameState::kPlayingState;
        return;
      }
      else if (overWindow.homeButton.IsClicked())
      {
        Reset();
        state = kGameState::kHomeState;
        return;
      }
      else if (overWindow.quitButton.IsClicked())
      {
        state = kGameState::kQuitState;
        return;
      }
      break;
    }
  }
}

void Game::PlaceFood()
{
  int x, y;
  while (true)
  {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y))
    {
      food = Food(x, y, random_label(engine));
      return;
    }
  }
}

void Game::Update()
{
  if (!snake.alive)
  {
    state = kGameState::kOverState;
    overWindow.messagebox.text = "Score: " + std::to_string(score);
    // If the game need to be logged
    if (logged)
    {
      // Log the game
      Log();
      overWindow.messagebox.text += ", High Score: " + std::to_string(high_score);
    }
    return;
  }

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.AtCell(new_x, new_y))
  {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

void Game::Reset()
{
  snake.Reset();
  score = 0;
  PlaceFood();
}

void Game::Log()
{
  // Create a filestream object for appending and reading
  std::fstream filestream(LOG_FILE_PATH, std::ios::app | std::ios::in);
  if (filestream.is_open())
  {
    // Write to log file
    // Get the current time
    time_t now = time(NULL);
    tm *t = gmtime(&now);
    char time[20];
    strftime(time, 20, "%FT%TZ", t);
    // Stream log message components to the file
    filestream << time << "Z | INFO | User: " << usr << ", Score: " << score << ", Snake_size: " << GetSize() << std::endl;

    // Read from log file
    std::string log_message;
    std::string token, susr;
    int sscore;
    filestream.seekg(std::ios::beg);
    // Read line by line
    while (std::getline(filestream, log_message))
    {
      std::replace(log_message.begin(), log_message.end(), '|', ' ');
      std::replace(log_message.begin(), log_message.end(), ',', ' ');
      std::replace(log_message.begin(), log_message.end(), ':', ' ');
      std::istringstream linestream(log_message);
      while (linestream >> token)
      {
        if (token == "User")
        {
          linestream >> susr >> token >> sscore;
          break;
        }
      }
      // Check if user appear in the message is current user
      if (susr == usr)
      {
        // Compare current high score with record
        if (sscore > high_score)
        {
          // If record greater, replace current with it
          high_score = sscore;
        }
      }
    }
  }

  // Close file after writing and reading
  filestream.close();
}

int Game::GetScore() const { return score; }
int Game::GetHighScore() const { return high_score; }
int Game::GetSize() const { return snake.size; }
std::string Game::GetUser() const { return usr; }
