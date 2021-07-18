#pragma once

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

class cEngine {
public:
  cEngine();
  virtual ~cEngine();

public:
  bool init(const char *title, int width, int heigth,
            int initFlags = SDL_INIT_VIDEO, int winFlags = SDL_INIT_VIDEO,
            int renderFlags = SDL_RENDERER_ACCELERATED);

  void start(bool escapeClose = true);
  virtual bool setup() = 0;
  virtual bool update() = 0;

protected:
  SDL_Window *m_window;
  SDL_Renderer *m_renderer;
  std::vector<SDL_Event> m_events;

  int m_screenWidth;
  int m_screenHeigth;
};
