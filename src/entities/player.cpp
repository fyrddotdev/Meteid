#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "player.h"
#include "gameUI.h"

Player::Player()
{
  speedX = 5;
  health = 10;
  score = 0;
  rect = {256, 550, 32, 32};
  origin = {0};
  rotation = 0.0f;
  isDead = false;
  damageInterval = 0.4f;
  damageTimer = 0.0f;
  instance = this;
}

Color playerColor = {0, 194, 200, 255};

void Player::Update()
{
  // Jika player mati, berhenti jalankan logic Update()
  if (isDead)
    return;

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

  // Teleport jika keluar layar
  rect.x = Wrap(rect.x, 0 - (rect.width / 2), GetScreenWidth() + (rect.width / 2)); // Pake Wrap() lebih efektif

  // if (rect.x < (0 - rect.width))
  // {
  //   rect.x = 360;
  // }
  // else if (rect.x > (GetScreenWidth() + rect.width))
  // {
  //   rect.x = 0 - rect.width + 1;
  // }
}

void Player::TriggerDamage(int damagePoint)
{
  // Jika player mati, berhenti menerima damage lagi
  if (isDead)
    return;

  health -= damagePoint;
  if (health <= 0)
  {
    Player::Get()->isDead = true;
  }

  // Flash layar / UI
  GameUI::flashOpacity = 1.0f;
  GameUI::flashColor = RED;
}
void Player::TriggerHeal(int healPoint)
{
  health += healPoint;
}

void Player::Draw() const
{
  // Jika player mati, berhenti melakukan draw pada player
  if (isDead)
    return;
  DrawRectanglePro(rect, origin, rotation, playerColor);
}