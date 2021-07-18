#include "gameEngine.hpp"

cGameEngine::cGameEngine() : cEngine(), m_player({300, 600}) {}

bool cGameEngine::setup() {

  std::srand(std::time(nullptr));

  // Player
  m_player.setRenderer(m_renderer);
  m_helth = 10;
  m_canTakeDamage = true;

  // Rope
  m_ropeX = 100;
  m_ropeBrocke = false;

  // Timing
  m_lastHit = -10.0f;
  m_lastSpawn = -5.0f;

  for (auto &k : m_keyPressed)
    k = false;

  SDL_GetWindowSize(m_window, &m_screeW, &m_screeH);

  return true;
}

bool cGameEngine::update() {

  m_currentFrame = SDL_GetTicks() / 1000.0f;

  int mouseX = 0, mouseY = 0;

  SDL_GetMouseState(&mouseX, &mouseY);
  for (auto e : m_events) {

    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
      case SDLK_z:
        m_keyPressed[key::Z] = true;
        break;
      case SDLK_q:
        m_keyPressed[key::Q] = true;
        break;
      case SDLK_s:
        m_keyPressed[key::S] = true;
        break;
      case SDLK_d:
        m_keyPressed[key::D] = true;
        break;
      case SDLK_SPACE:
        m_keyPressed[key::SPACE] = true;
        break;
      }
    }

    if (e.type == SDL_KEYUP) {
      switch (e.key.keysym.sym) {
      case SDLK_z:
        m_keyPressed[key::Z] = false;
        break;
      case SDLK_q:
        m_keyPressed[key::Q] = false;
        break;
      case SDLK_s:
        m_keyPressed[key::S] = false;
        break;
      case SDLK_d:
        m_keyPressed[key::D] = false;
        break;
      case SDLK_SPACE:
        m_keyPressed[key::SPACE] = false;
        break;
      }
    }

    if (e.type == SDL_MOUSEBUTTONDOWN) {
      sVec2f playerPos = m_player.getPos();

      sVec2f dir;
      dir.x = mouseX - playerPos.x;
      dir.y = mouseY - playerPos.y;
      dir.normalize();

      sVec2f pos = {playerPos.x + 20 + dir.x * 33,
                    playerPos.y + 20 + dir.y * 33};

      m_projectiles.push_back(cProjectile(m_renderer, pos, dir));
    }
  }

  // Spawnig
  spawn();

  // Movements
  m_player.move(m_keyPressed);
  for (auto &enemy : m_enemys)
    enemy.move();

  for (auto &p : m_projectiles)
    p.move();

  // Colisions
  colisionTest();

  // Render
  // ------
  SDL_SetRenderDrawColor(m_renderer, 51, 51, 51, 255);
  SDL_RenderClear(m_renderer);

  // Rope
  SDL_SetRenderDrawColor(m_renderer, 255, 200, 100, 255);
  SDL_RenderDrawLine(m_renderer, m_ropeX, 0, m_ropeX, m_screeH - 200);
  SDL_RenderDrawLine(m_renderer, m_ropeX + 1, 0, m_ropeX + 1, m_screeH - 200);

  SDL_Rect stone = {m_ropeX - 50, m_screeH - 200, 100, 60};
  SDL_SetRenderDrawColor(m_renderer, 20, 25, 30, 255);
  SDL_RenderFillRect(m_renderer, &stone);

  // Helth
  SDL_Rect helthRect = {400, m_screeH - 80, 40, 40};
  SDL_SetRenderDrawColor(m_renderer, 200, 200, 0, 255);
  for (int i = 0; i < m_helth; i++) {
    SDL_RenderFillRect(m_renderer, &helthRect);
    helthRect.x += helthRect.w + 3;
  }

  // Battery + calbles
  Uint8 r, g;
  SDL_Rect playerRect = m_player.getRect();
  SDL_Rect bateryRect = {m_ropeX - 20, m_screeH - 80, 40, 40};

  r = (std::sin(m_currentFrame) * 0.25f + 0.25f) * 255 + 128;
  g = (std::cos(m_currentFrame) * 0.25f + 0.25f) * 255 + 128;

  SDL_SetRenderDrawColor(m_renderer, 255, 200, 50, 255);
  SDL_RenderDrawLine(m_renderer, bateryRect.w + bateryRect.w,
                     bateryRect.y + bateryRect.h / 2, bateryRect.x + 200,
                     bateryRect.y + bateryRect.h / 2);

  SDL_RenderDrawLine(
      m_renderer, bateryRect.x + 200, bateryRect.y + bateryRect.h / 2,
      playerRect.x + playerRect.w / 2, playerRect.y + playerRect.h / 2);

  SDL_SetRenderDrawColor(m_renderer, r, g, 0, 255);
  SDL_RenderFillRect(m_renderer, &bateryRect);

  // Player
  m_player.render(mouseX, mouseY);

  // Enemys
  for (auto &enemy : m_enemys)
    enemy.render();

  // Projectiles
  for (auto &p : m_projectiles)
    p.render();

  SDL_RenderPresent(m_renderer);

  if (endGame()) {
    gameOver();
    return false;
  }

  return true;
}

void cGameEngine::spawn() {
  int w, h;
  SDL_GetWindowSize(m_window, &w, &h);

  if (m_currentFrame - m_lastSpawn >
      8.0f - ((m_currentFrame * 7) / (m_currentFrame + 7))) {
    m_enemys.push_back(cEnemy(m_renderer, w, std::rand() % 480 + 20));
    m_lastSpawn = m_currentFrame;
  }
}

void cGameEngine::colisionTest() {

  for (int i = 0; i < m_projectiles.size(); i++) {
    // Projectile - Wall
    SDL_Rect projRect = m_projectiles[i].getRect();
    bool move = false;

    if (projRect.y <= 0 || projRect.y + projRect.h >= m_screeH) {
      m_projectiles[i].bounce(VERTICAL);
      move = true;
    }
    if (projRect.x <= 0 || projRect.x + projRect.w >= m_screeW) {
      m_projectiles[i].bounce(HORIZONTAL);
      move = true;
    }

    // Projectile - Enemy
    for (int j = 0; j < m_enemys.size(); j++) {
      std::vector<SDL_Rect> shields = m_enemys[j].getShields();
      SDL_Rect weak = m_enemys[j].getWeak();

      for (auto &s : shields) {
        int hit = colisionSide(projRect, s);

        if (hit & UP || hit & DOWN) {
          m_projectiles[i].bounce(VERTICAL);
          move = true;
        }

        if (hit & LEFT || hit & RIGTH) {
          m_projectiles[i].bounce(HORIZONTAL);
          move = true;
        }
      }

      if (colide(projRect, weak))
        m_enemys.erase(m_enemys.begin() + j);
    }

    if (move)
      m_projectiles[i].move();

    if (m_projectiles[i].getHelth() <= 0)
      m_projectiles.erase(m_projectiles.begin() + i);
  }

  // Enemy - Rope
  for (auto &enemy : m_enemys) {
    if (enemy.getShields()[0].x <= m_ropeX)
      m_ropeBrocke = true;
  }

  // Player - Wall
  SDL_Rect playerRect = m_player.getRect();

  if (playerRect.x < 200)
    m_player.setPos(200, playerRect.y);
  if (playerRect.x + playerRect.w > m_screeW)
    m_player.setPos(m_screeW - playerRect.w, playerRect.y);
  if (playerRect.y < 0)
    m_player.setPos(playerRect.x, 0);
  if (playerRect.y + playerRect.h > m_screeH)
    m_player.setPos(playerRect.x, m_screeH - playerRect.h);

  // Enemy - Player
  for (auto &enemy : m_enemys) {
    std::vector<SDL_Rect> shields = enemy.getShields();

    if (m_currentFrame - m_lastHit > 1.0f) {
      for (auto &s : shields) {
        if (colide(playerRect, s)) {
          m_helth--;
          m_lastHit = m_currentFrame;
          break;
        }
      }
    }
  }
}

bool cGameEngine::endGame() { return m_ropeBrocke || m_helth <= 0; }

void cGameEngine::gameOver() {

  for (int i = 0; i < 6; i++) {
    SDL_SetRenderDrawColor(m_renderer, 51, 51, 51, 255);
    SDL_RenderClear(m_renderer);

    // Rope
    SDL_SetRenderDrawColor(m_renderer, 255, 200, 100, 255);
    SDL_RenderDrawLine(m_renderer, m_ropeX, 0, m_ropeX, m_screeH - 200);
    SDL_RenderDrawLine(m_renderer, m_ropeX + 1, 0, m_ropeX + 1, m_screeH - 200);

    // Helth
    SDL_Rect helthRect = {400, m_screeH - 80, 40, 40};
    SDL_SetRenderDrawColor(m_renderer, 200, 200, 0, 255);
    for (int i = 0; i < m_helth; i++) {
      SDL_RenderFillRect(m_renderer, &helthRect);
      helthRect.x += helthRect.w + 3;
    }

    // Battery + calbles
    Uint8 r, g;
    SDL_Rect playerRect = m_player.getRect();
    SDL_Rect bateryRect = {m_ropeX - 20, m_screeH - 80, 40, 40};

    r = (std::sin(m_currentFrame) * 0.25f + 0.25f) * 255 + 128;
    g = (std::cos(m_currentFrame) * 0.25f + 0.25f) * 255 + 128;

    SDL_SetRenderDrawColor(m_renderer, 255, 200, 50, 255);
    SDL_RenderDrawLine(m_renderer, bateryRect.w + bateryRect.w,
                       bateryRect.y + bateryRect.h / 2, bateryRect.x + 200,
                       bateryRect.y + bateryRect.h / 2);

    SDL_RenderDrawLine(
        m_renderer, bateryRect.x + 200, bateryRect.y + bateryRect.h / 2,
        playerRect.x + playerRect.w / 2, playerRect.y + playerRect.h / 2);

    SDL_SetRenderDrawColor(m_renderer, r, g, 0, 255);
    SDL_RenderFillRect(m_renderer, &bateryRect);

    // Player
    m_player.render(1, 0);

    // Enemys
    for (auto &enemy : m_enemys)
      enemy.render();

    // Projectiles
    for (auto &p : m_projectiles)
      p.render();

    // Stone
    SDL_Rect stone = {m_ropeX - 50, m_screeH - 200 + i * 20, 100, 60};
    SDL_SetRenderDrawColor(m_renderer, 20, 25, 30, 255);
    SDL_RenderFillRect(m_renderer, &stone);

    SDL_RenderPresent(m_renderer);

    SDL_Delay(500);
  }

  SDL_Delay(500);

  SDL_Surface *texSurface = SDL_LoadBMP("gameOver.bmp");
  SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, texSurface);

  SDL_RenderCopy(m_renderer, texture, NULL, NULL);
  SDL_RenderPresent(m_renderer);

  SDL_Delay(5000);
}
