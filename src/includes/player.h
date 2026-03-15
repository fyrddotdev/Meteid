#pragma once
#include <raylib.h>
class Player

{
public:
  Player();
  void Update();
  void Draw() const;
  void TriggerDamage(int damagePoint);
  void TriggerHeal(int healPoint);

  int speedX;
  int health;
  int score;

  Rectangle rect;

  Rectangle playerCollisionRect;

private:
  Vector2 origin;
  float rotation;

  bool gotHit;
  float damageInterval;
  float damageTimer;
};