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

  Rectangle rect;

  Rectangle playerCollisionRect;

private:
  inline static Player *instance = nullptr;
  Vector2 origin;
  float rotation;

  float damageInterval;
  float damageTimer;
};