#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "player.h"
#include "playerExplosion.h"

#include "gameUI.h"

extern float shakeDuration;
extern float shakeIntensity;

float decayTimer = 3.0f;
float rotationSpeed = 0.5f;

Player::Player()
{
  speedX = 5;
  health = 1;
  score = 0;
  rect = {256, 550, 32, 32};
  origin = {0};
  rotation = 0.0f;
  isDead = false;
  afterDecay = false;
  damageInterval = 0.4f;
  damageTimer = 0.0f;
  instance = this;
}

Color playerColor = {0, 194, 200, 255};

void Player::Update()
{
  // If player is dead, stop executing Update() logic
  if (!isDead)
  {
    // Constantly calculate origin and collision boundaries
    origin = {
        rect.width / 2,
        rect.height / 2};

    playerCollisionRect = {
        rect.x - rect.width / 2,
        rect.y - rect.height / 2,
        rect.width,
        rect.height};

    // Touchscreen support
    bool isTouching = (GetTouchPointCount() > 0);
    float touchX = GetTouchX();

    // Movement and Rotation logic
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) || (isTouching && touchX < GetScreenWidth() / 2))
    {
      rect.x -= speedX;
      rotation -= speedX;
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || (isTouching && touchX > GetScreenWidth() / 2))
    {
      rect.x += speedX;
      rotation += speedX;
    }

    // Wrap around the screen if the player goes off-bounds
    rect.x = Wrap(rect.x, 0 - (rect.width / 2), GetScreenWidth() + (rect.width / 2));
  }

  // Death / Decay Animation
  if (isDead && !afterDecay)
  {
    rect.y = Lerp(rect.y, 450, 0.025f);
    rotation = Wrap(rotation + rotationSpeed, 0, 360);
    if (rotationSpeed < 200.0f)
    {
      rotationSpeed += 0.2f;
    }
    if (decayTimer > 0)
    {
      decayTimer -= GetFrameTime();
    }

    if (rect.y < 450 + 1 && decayTimer <= 0)
    {
      if (!PlayerExplosion::Get()->enabled)
      {
        GameUI::flashOpacity = 1.0f;
        GameUI::flashColor = WHITE;
        shakeDuration = 5.0f;
        shakeIntensity = 8.0f;
        PlayerExplosion::Get()->enabled = true;
      }
      afterDecay = true;
    }
  }

  // Trigger GameOver state once the explosion animation completes
  if (PlayerExplosion::Get()->isComplete)
  {
    GameUI::Get()->currState = GameUI::gameState::GAMEOVER;
  }
}

void Player::TriggerDamage(int damagePoint)
{
  // Prevent taking damage if the player is already dead
  if (isDead)
    return;

  health -= damagePoint;
  if (health <= 0)
  {
    Player::Get()->isDead = true;
  }

  // Trigger screen / UI flash effect
  GameUI::flashOpacity = 1.0f;
  GameUI::flashColor = RED;
}

void Player::TriggerHeal(int healPoint)
{
  health += healPoint;
}

void Player::Draw() const
{
  // Stop drawing the player entity once the decay process is finished
  if (!afterDecay)
  {
    DrawRectanglePro(rect, origin, rotation, playerColor);
  }
}