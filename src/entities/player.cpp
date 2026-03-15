#include <raylib.h>
#include <iostream>
#include "player.h"

Player::Player()
    : speedX(5), health(4), score(0), rect{256, 550, 32, 32} // x, y, sizeX, sizeY
      ,
      origin{0}, rotation(0), gotHit(false), damageInterval(0.4f), damageTimer(0.0f)
{
}

Color playerColor = {0, 194, 200, 255};

void Player::Update()
{
  // Melakukan kalkulasi terus menerus terhadap origin dan collision
  origin = {
      rect.width / 2,
      rect.height / 2};

  playerCollisionRect = {
      rect.x - rect.width / 2,
      rect.y - rect.height / 2,
      rect.width,
      rect.height};

  // Movement dan Rotation
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
  {
    rect.x -= speedX;
    rotation -= speedX;
  }
  else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
  {
    rect.x += speedX;
    rotation += speedX;
  }

  // Teleport jika keluar scene
  if (rect.x < (0 - rect.width))
  {
    rect.x = 360;
  }
  else if (rect.x > (GetScreenWidth() + rect.width))
  {
    rect.x = 0 - rect.width + 1;
  }

  // Cek jika player terkena hit, dia akan melakukan reset pada variabel bool gotHit.

  // if (gotHit)
  // {
  //   std::cout << "Player reset damage timer!" << std::endl;
  //   damageTimer = GetFrameTime();
  //   if (damageTimer < damageInterval)
  //   {
  //     gotHit = false;
  //     std::cout << "Player reset completed!" << std::endl;
  //     damageTimer = 0;
  //   }
  // }
}

void Player::TriggerDamage(int damagePoint)
{
  health -= damagePoint;
  // if (!gotHit)
  // {
  //   gotHit = true;
  //   std::cout << "Player hit!" << std::endl;
  // }
}
void Player::TriggerHeal(int healPoint)
{
  health += healPoint;
}

void Player::Draw() const
{
  DrawRectanglePro(rect, origin, rotation, playerColor);
}