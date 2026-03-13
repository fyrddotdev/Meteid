#include <raylib.h>
#include <iostream>
#include "player.h"

Player::Player()
    : speedX(5), rect{256, 550, 32, 32} // x, y, sizeX, sizeY
      ,
      origin{0}, rotation(0)
{
}

Color playerColor = {0, 194, 200, 255};

void Player::Update()
{
  // Melakukan kalkulasi terus menerus origin
  origin = {
      rect.width / 2,
      rect.height / 2};

  // Movement dan Rotation ( 263 = Left, 262 = Right )
  if (IsKeyDown(263))
  {
    rect.x -= speedX;
    rotation -= speedX;
  }
  else if (IsKeyDown(262))
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
}

void Player::Draw() const
{
  DrawRectanglePro(rect, origin, rotation, playerColor);
}