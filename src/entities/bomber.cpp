#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>

#include "bomber.h"

std::vector<Texture2D> Bomber::bomberTextures;
float animTimer = 0.0f;
float animSpeed = 1.0f;
int currentFrame = 0;
int maxFrame;

Rectangle bomberDest;
int minX;
int maxX;

Bomber::Bomb bomb;
float bombTimer;
float bombInterval = GetRandomValue(50, 150) / 10.0f;

Bomber::Bomber()
{
  // Memuat semua texture yang dibutuhkan
  bomberTextures = {
      LoadTexture("src/assets/graphics/enemies/bomber_0.png"),
      LoadTexture("src/assets/graphics/enemies/bomber_1.png"),
      LoadTexture("src/assets/graphics/enemies/bomber_2.png")};
  bomb.texture = LoadTexture("src/assets/graphics/enemies/bomb.png");

  // Initialisasi bomb
  bomb.maxSpeed = 10.0f;
  bomb.acceleration = 0.1f;
  bomb.speed = 0.0f;

  // Setelah memuat, tentukan MaxFrame dari bomberTexture
  maxFrame = bomberTextures.size();

  // Masukan nilai variabel yang akan dibutuhkan untuk movement Bomber
  minX = 0 + size;
  maxX = GetScreenWidth() - size;
  x = abs((minX + maxX) / 2);
  y = 0 - size;
}

void Bomber::Update()
{
  // Update terus bomberDest
  bomberDest = {x, y, size, size};
  // std::cout << y << "\n";

  // Animasi bomber masuk layar
  y = Lerp(y, 80, 0.05f);

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

  // Logic untuk movement bomber
  if (y > 79)
  {
    x += speedX;
  }

  if (speedX < 0 && abs(x - minX) < 1)
  {
    speedX = -1 * speedX;
  }
  if (speedX > 0 && abs(x - maxX) < 1)
  {
    speedX = -speedX;
  }

  // Logic untuk drop bomb
  bombTimer += GetFrameTime();

  if (abs(bombInterval - bombTimer) < 2.0f)
  {
    // Perlambat jika selisih bombInterval dan bombTimer dibawah 1.
    if (speedX < 0)
      speedX = -0.2f;
    if (speedX > 0)
      speedX = 0.2f;
  }

  if (bombTimer >= bombInterval)
  {
    // Reset speed player, membalikkan arah bomber bergerak setiap kali mengeluarkan bomb
    if (speedX > 0)
      speedX = -1.0f;
    if (speedX < 0)
      speedX = 1.0f;

    // Nyalakan bomb dan menginitialisasi variabel yang dibutuhkan
    bomb.isActive = true;
    bomb.spawnPosition = {bomberDest.x, bomberDest.y + size / 4};

    // Reset timer
    bombInterval = (float)GetRandomValue(50, 150) / 10.0f;
    bombTimer = 0.0f;

    // std::cout << "spawned" << "\n";
  }
  if (bomb.isActive)
  {
    bomb.rotation += 5.0f;
    bomb.speed += bomb.acceleration;
    if (bomb.speed > bomb.maxSpeed)
      bomb.speed = bomb.maxSpeed;

    bomb.spawnPosition.y += bomb.speed;
    // std::cout << bomb.speed << "\n";

    // Matikan bom jika keluar layar
    if (bomb.spawnPosition.y > GetScreenHeight())
    {
      bomb.isActive = false;
      // Reset state
      bomb.maxSpeed = 10.0f;
      bomb.acceleration = 0.1f;
      bomb.speed = 0.0f;
      bomb.rotation = 0.0f;
    }
  }
}

void Bomber::Draw() const
{
  if (bomberTextures.size() != 0)
    if (bomb.isActive)
    {
      Rectangle dest = {bomb.spawnPosition.x, bomb.spawnPosition.y, (float)bomb.texture.width * 2, (float)bomb.texture.height * 2};

      DrawTexturePro(bomb.texture, {0, 0, (float)bomb.texture.width, (float)bomb.texture.height}, dest, {dest.width / 2, dest.height / 2}, bomb.rotation, WHITE);
    }
  {
    DrawTexturePro(bomberTextures[currentFrame], {0, 0, 16, 16}, bomberDest, {size / 2, size / 2}, 0, WHITE);
  }
}