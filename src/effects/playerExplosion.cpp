#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "Global.h"

#include "playerExplosion.h"
#include "player.h"

Rectangle PeSource;
Rectangle PeDest;

Rectangle PeLSource;
Rectangle PeLDest;

std::vector<Texture2D> PlayerExplosion::textures;

PlayerExplosion::PlayerExplosion()
{
  scale = 0.0f;
  fadeValue = 1.0f;
  enabled = false;
  isComplete = false;
  instance = this;
}

void PlayerExplosion::Init()
{
  textures = {
      LoadTexturePixel(ASSETS_PATH "effects/playerExplodeEffect.png"),
      LoadTexturePixel(ASSETS_PATH "effects/lightExplodeEffect.png")};
  PeSource = {0,
              0,
              (float)textures[0].width,
              (float)textures[0].height};
  PeLSource = {0,
               0,
               (float)textures[1].width,
               (float)textures[1].height};
}

void PlayerExplosion::Update()
{
  if (enabled && !isComplete)
  {
    scale += 0.025f;
    fadeValue -= 0.01f;
    rotation = Wrap(rotation + 1.0f, 0, 360);

    // Update rect size and position every frame
    if (Player::Get() != nullptr)
    {
      PeDest.x = Player::Get()->rect.x - Player::Get()->rect.width / 2;
      PeLDest.x = Player::Get()->rect.x - Player::Get()->rect.width / 2;
      PeDest.y = Player::Get()->rect.y - Player::Get()->rect.height / 2;
      PeLDest.y = Player::Get()->rect.y - Player::Get()->rect.height / 2;
    }
    PeDest.width = (float)textures[0].width * scale;
    PeDest.height = (float)textures[0].height * scale;
    PeLDest.width = (float)textures[1].width * scale;
    PeLDest.height = (float)textures[1].height * scale;

    if (fadeValue <= 0)
      isComplete = true;
  }
}

void PlayerExplosion::Draw() const
{
  if (enabled && !isComplete)
  {
    DrawTexturePro(textures[0], PeSource, PeDest, {PeDest.width / 2, PeDest.height / 2}, rotation, Fade(WHITE, fadeValue));
    DrawTexturePro(textures[1], PeLSource, PeLDest, {PeLDest.width / 2, PeLDest.height / 2}, rotation, Fade(WHITE, fadeValue));
  }
}