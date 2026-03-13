#include <iostream>

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "player.h"
#include "meteor.h"

int main()
{
  // Initialisasi GameWindow
  constexpr int screenWidth = 360;
  constexpr int screenHeight = 640;
  Vector2 screenCenter = {screenWidth / 2, screenHeight / 2};
  const Color backgroundColor = {35, 26, 63, 255};

  InitWindow(screenWidth, screenHeight, "METEID! Raylib edition");
  SetTargetFPS(60);

  // Initialisasi Camera
  Camera2D camera = {0};
  camera.offset = {screenWidth / 2, screenHeight / 2};
  camera.target = {screenWidth / 2, screenHeight / 2};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  // Initialisasi Player
  Player player;

  // Initialisasi Meteor
  std::vector<Meteor> meteors;

  Meteor::meteorTexture = LoadTexture("src/res/meteorid.png");
  Meteor meteor;
  float spawnTimer = 0.0f;
  float spawnInterval = GetRandomValue(2.5, 20) / 10.0f;

  // Initialisasi Game Environtment
  const Rectangle PlayerLine = {-32, player.rect.y - 4, screenWidth + 64, 8};

  // Game loop Logic
  while (!WindowShouldClose())
  {
    // Sistem spawn meteor dengan interval <SpawnInterval> detik
    spawnTimer += GetFrameTime();
    if (spawnTimer >= spawnInterval)
    {
      meteors.push_back(Meteor());
      spawnTimer = 0;
      spawnInterval = GetRandomValue(2.5, 20) / 10.0f;
      // std::cout << "Meteor Timer triggered" << std::endl;
    }

    // Update
    player.Update();

    // For loop untuk cek dan update setiap meteor[i] didalam meteors container
    for (size_t i = 0; i < meteors.size(); i++)
    {
      meteors[i].Update();

      // Jika meteor[i] diatas playerLine.y, maka hapus meteor[i] didalam meteors container
      if (meteors[i].y > screenHeight + meteors[i].meteorTexture.height)
      {
        meteors.erase(meteors.begin() + i);
        i--;
      }

      // Jika meteor menabrak garis batas player, maka hapus meteor[i] didalam meteors container
      if (CheckCollisionCircleRec(meteors[i].center, meteors[i].radius, PlayerLine))
      {
        // Efek fade & scale sebelum benar-benar dihancurkan
        meteors[i].speedX = 0;
        meteors[i].speedY = 0;
        int fadeSubtraction = 10;
        if (meteors[i].color.a > fadeSubtraction)
        {
          meteors[i].color.a -= fadeSubtraction;
          meteors[i].scale += 0.01f;
        }
        else
        {
          meteors.erase(meteors.begin() + i);
          i--;
        }
        // Efek getar pada kamera diakibatkan tabrakan
        float shakeDuration = 0.5f;
        float shakeIntensity = 4.0f;
        if (shakeDuration > 0)
        {
          shakeDuration -= GetFrameTime();

          camera.offset.x = screenCenter.x + GetRandomValue(-shakeIntensity, shakeIntensity);
          camera.offset.y = screenCenter.y + GetRandomValue(-shakeIntensity, shakeIntensity);
        }
        {
          camera.offset.x = Lerp(camera.offset.x, screenCenter.x, 0.02f);
          camera.offset.y = Lerp(camera.offset.y, screenCenter.y, 0.02f);

          // Paksa camera.offset berada pada titik tengah
          if (CheckCollisionPointCircle(camera.offset, screenCenter, 0.1f))
          {
            camera.offset = screenCenter;
          }
          // std::cout << camera.offset.x << " : " << camera.offset.y << std::endl;
        }
      }

      // Jika meteor menabrak player, maka game over
      Rectangle playerCollisionRect = {
          player.rect.x - player.rect.width / 2,
          player.rect.y - player.rect.height / 2,
          player.rect.width,
          player.rect.height};

      if (CheckCollisionCircleRec(meteors[i].center, meteors[i].radius, playerCollisionRect))
      {
        // Pass here
      }
    }

    // Drawing logic
    BeginDrawing();
    BeginMode2D(camera);

    // Menggambar environtment
    ClearBackground(backgroundColor);
    DrawRectangleRec(PlayerLine, WHITE);

    // Menggambar object seperti player, meteors, dll
    player.Draw();

    // Spawn setiap meteor
    for (auto &meteor : meteors)
    {
      meteor.Draw();
    }
    EndMode2D();
    EndDrawing();
  }
  CloseWindow();
}