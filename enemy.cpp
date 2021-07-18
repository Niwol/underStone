#include "enemy.hpp"

cEnemy::cEnemy(SDL_Renderer *renderer, float x, float y)
    : m_speed(2.0f), m_renderer(renderer) {

  m_position.x = x;
  m_position.y = y;

  int px = m_position.x;
  int py = m_position.y;

  m_weak = {px + 40, py + 40, 15, 15};

  m_shields.push_back({px, py + 10, 15, 70});
  m_shields.push_back({px + 5, py + 5, 60, 15});
  m_shields.push_back({px + 5, py + 70, 60, 15});
}

void cEnemy::move() {
  m_position.x -= m_speed;
  m_weak.x -= m_speed;

  for (auto &r : m_shields)
    r.x -= m_speed;
}

void cEnemy::render() {
  SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(m_renderer, &m_weak);

  SDL_SetRenderDrawColor(m_renderer, 0, 0, 255, 255);
  for (auto &r : m_shields)
    SDL_RenderFillRect(m_renderer, &r);

  SDL_RenderDrawLine(m_renderer, m_position.x, m_position.y + 44,
                     m_position.x + 40, m_position.y + 44);
  SDL_RenderDrawLine(m_renderer, m_position.x, m_position.y + 45,
                     m_position.x + 40, m_position.y + 45);
}
