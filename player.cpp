#include "player.hpp"

cPlayer::cPlayer(sVec2f pos) : m_position(pos), m_size(40), m_speed(6) {}

void cPlayer::move(bool keyPressed[key::TOTAL]) {

  if (keyPressed[key::Z])
    m_position.y -= m_speed;
  if (keyPressed[key::S])
    m_position.y += m_speed;
  if (keyPressed[key::Q])
    m_position.x -= m_speed;
  if (keyPressed[key::D])
    m_position.x += m_speed;
}

void cPlayer::render(int mouseX, int mouseY) {

  SDL_Rect rect = {(int)m_position.x, (int)m_position.y, m_size, m_size};

  SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
  SDL_RenderFillRect(m_renderer, &rect);

  sVec2f canonTip = {mouseX - m_position.x, mouseY - m_position.y};
  canonTip.normalize();
  canonTip.x *= 20;
  canonTip.y *= 20;

  SDL_SetRenderDrawColor(m_renderer, 100, 100, 200, 255);

  for (int i = -1; i <= 1; i++)
    SDL_RenderDrawLine(m_renderer, (int)m_position.x + m_size / 2 + i,
                       (int)m_position.y + m_size / 2 + i,
                       (int)m_position.x + m_size / 2 + (int)canonTip.x + i,
                       (int)m_position.y + m_size / 2 + (int)canonTip.y + i);
}

void cPlayer::setRenderer(SDL_Renderer *renderer) { m_renderer = renderer; }
