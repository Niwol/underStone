#pragma once

#include <cmath>

#include <SDL2/SDL.h>

constexpr int SCREEN_W = 1500;
constexpr int SCREEN_H = 800;

enum { UP = 1, DOWN = 2, LEFT = 4, RIGTH = 8 };
enum { VERTICAL, HORIZONTAL };
enum key { Z, Q, S, D, SPACE, TOTAL };

struct sVec2f {
  float x, y;

  inline float norm() { return std::sqrt(x * x + y * y); }
  inline void normalize() {
    float n = norm();
    x /= n;
    y /= n;
  }
};

inline bool colide(SDL_Rect &r1, SDL_Rect &r2) {
  return !((r1.x + r1.w < r2.x || r1.x > r2.x + r2.w) ||
           (r1.y + r1.h < r2.y || r1.y > r2.y + r2.h));
}

inline int colisionSide(SDL_Rect &rect, SDL_Rect &hitRect) {

  int side = 0;
  if (colide(rect, hitRect)) {

    if (rect.x < hitRect.x + rect.w * 3)
      side |= LEFT;
    if (rect.x + rect.w > hitRect.x + hitRect.w - rect.w * 3)
      side |= RIGTH;

    if (rect.y < hitRect.y + rect.h * 3)
      side |= UP;
    if (rect.y + rect.h > hitRect.y + hitRect.h - rect.h * 3)
      side |= DOWN;
  }

  return side;
}
