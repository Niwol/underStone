#pragma once

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

#include "common.hpp"

class cEnemy {

public:
  cEnemy(SDL_Renderer *renderer, float x, float y);

public:
  void move();
  void render();

  inline std::vector<SDL_Rect> getShields() { return m_shields; }
  inline SDL_Rect getWeak() { return m_weak; }

private:
  sVec2f m_position;
  float m_speed;

  std::vector<SDL_Rect> m_shields;
  SDL_Rect m_weak;

  SDL_Renderer *m_renderer;
};
