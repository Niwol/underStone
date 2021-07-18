#pragma once

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

#include "common.hpp"
#include "projectile.hpp"

class cPlayer {
public:
  cPlayer(sVec2f pos);

public:
  void move(bool keyPressed[key::TOTAL]);
  void fire(std::vector<cProjectile> &projectiles);
  void render(int mouseX, int mouseY);

public:
  void setRenderer(SDL_Renderer *renderer);

  inline void setPos(int x, int y) {
    m_position.x = x;
    m_position.y = y;
  }

  inline sVec2f getPos() { return m_position; }
  inline SDL_Rect getRect() {
    return {(int)m_position.x, (int)m_position.y, m_size, m_size};
  }

private:
  sVec2f m_position;
  int m_size;
  float m_speed;

  SDL_Renderer *m_renderer;
};