#include "projectile.hpp"

cProjectile::cProjectile(SDL_Renderer *renderer, sVec2f position,
                         sVec2f direction)
    : m_position(position), m_direction(direction), m_helth(5), m_speed(10),
      m_renderer(renderer) {
  m_direction.normalize();
}

void cProjectile::move() {
  m_position.x += m_direction.x * m_speed;
  m_position.y += m_direction.y * m_speed;
}

void cProjectile::bounce(int dir) {
  if (dir == VERTICAL)
    m_direction.y *= -1;
  else if (dir == HORIZONTAL)
    m_direction.x *= -1;

  m_helth--;
}

void cProjectile::render() {
  SDL_Rect r = {(int)m_position.x, (int)m_position.y, 3, 3};

  SDL_SetRenderDrawColor(m_renderer, 200, 30, 100, 255);
  SDL_RenderFillRect(m_renderer, &r);
}
