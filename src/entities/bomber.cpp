#include <raylib.h>
#include <iostream>
#include <vector>

#include "bomber.h"

std::vector<Texture2D> Bomber::bomberTextures;
float animTimer = 0.0f;
float animSpeed = 1.0f;
int currentFrame = 0;
int maxFrame;

Bomber::Bomber()
{
  // Memuat semua texture yang dibutuhkan
  bomberTextures = {
      LoadTexture("src/assets/graphics/enemies/bomber_0.png"),
      LoadTexture("src/assets/graphics/enemies/bomber_1.png"),
      LoadTexture("src/assets/graphics/enemies/bomber_2.png")};

  // Setelah memuat, tentukan MaxFrame dari bomberTexture
  maxFrame = bomberTextures.size();
}

void Bomber::Update()
{

  // Logic untuk animasi ( Planned to make this feature a modular script but not now... )
  animTimer += GetFrameTime();
  if (animTimer >= animSpeed)
  {
    if (currentFrame < maxFrame)
      currentFrame += 1;
    if (currentFrame == maxFrame)
      currentFrame = 0;

    animTimer = 0.0f;
  }
}

void Bomber::Draw() const
{
  if (bomberTextures.size() != 0)
  {
    DrawTexturePro(bomberTextures[currentFrame], {0, 0, 16, 16}, {(float)GetScreenWidth() / 2, 64, size, size}, {size / 2, size / 2}, 0, WHITE);
  }
}