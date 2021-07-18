#include <iostream>

#include <SDL2/SDL.h>

#include "gameEngine.hpp"

#include "common.hpp"

int main(int argc, char *argv[]) {

  cGameEngine engine;

  if (engine.init("Game", SCREEN_W, SCREEN_H, SDL_INIT_VIDEO,
                  SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN,
                  SDL_RENDERER_PRESENTVSYNC))
    engine.start();

  return 0;
}