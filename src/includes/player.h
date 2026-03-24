#pragma once
#include <raylib.h>
class Player

{
public:
  Player();
  void Update();
  void Draw() const;
  static Player *Get() { return instance; }
  void TriggerDamage(int damagePoint);
  void TriggerHeal(int healPoint);

  int speedX;
  int health;
  int score;
  bool isDead;
  Vector2 origin;

  Rectangle rect;
  Rectangle playerCollisionRect;

private:
  inline static Player *instance = nullptr;
  float rotation;

  bool afterDecay;

  float damageInterval;
  float damageTimer;
};