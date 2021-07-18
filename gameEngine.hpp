#pragma once

#include <ctime>
#include <iostream>
#include <vector>

#include "enemy.hpp"
#include "engine.hpp"
#include "player.hpp"
#include "projectile.hpp"

class cGameEngine : public cEngine {
public:
  cGameEngine();

public:
  virtual bool setup() override;
  virtual bool update() override;

  void spawn();
  void colisionTest();
  bool endGame();

  void gameOver();

private:
  cPlayer m_player;
  int m_helth;
  bool m_canTakeDamage;

  // Rope
  int m_ropeX;
  bool m_ropeBrocke;

  // Timing
  float m_currentFrame;
  float m_lastHit;
  float m_lastSpawn;

  std::vector<cEnemy> m_enemys;

  std::vector<cProjectile> m_projectiles;

  bool m_keyPressed[key::TOTAL];

  int m_screeW;
  int m_screeH;
};
