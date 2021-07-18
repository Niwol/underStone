#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#include "common.hpp"

class cProjectile {
public:
  cProjectile(SDL_Renderer *renderer, sVec2f position, sVec2f direction);

public:
  void move();
  void bounce(int dir);
  void render();

  inline SDL_Rect getRect() {
    return {(int)m_position.x, (int)m_position.y, 3, 3};
  }
  inline int getHelth() { return m_helth; }

private:
  sVec2f m_position;
  sVec2f m_direction;

  int m_helth;
  float m_speed;

  SDL_Renderer *m_renderer;
};
