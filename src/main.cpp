#include <iostream>

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "meteor.h"
#include "player.h"

#define RAYGUI_IMPLEMENTATION
#include <utils/raygui.h>
#include "gameUI.h"

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

  // Initialisasi variabel untuk effect camera shake
  float shakeDuration = 0.0f;
  float shakeIntensity = 4.0f;

  // Initialisasi GameUI
  GameUI gameUI;
  GameUI::gameState stateChange = GameUI::gameState::INGAME;
  GuiLoadStyle("src/assets/styles/meteid!.rgs");

  // Initialisasi Player
  Player player;

  // Initialisasi Meteor
  std::vector<Meteor> meteors;

  Meteor::meteorTexture = LoadTexture("src/assets/graphics/meteorid.png");
  Meteor meteor;
  float spawnTimer = 0.0f;
  float spawnInterval = GetRandomValue(2.5, 20) / 10.0f;

  // Initialisasi Game Environtment
  const Rectangle PlayerLine = {-32, player.rect.y - 4, screenWidth + 64, 8};

  // Game loop Logic
  while (!WindowShouldClose())
  {

    // Inisialisasi paket di frame ini
    GameUI::UIPacket frameData;

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

      if (meteors[i].state == MeteorState::FALLING) // Cek jika meteor sudah dalam keadaan HIT
      {

        // Jika meteor menabrak garis batas player, maka ubah state meteor ke state DECAYING
        if (CheckCollisionCircleRec(meteors[i].center, meteors[i].radius, PlayerLine))
        {
          meteors[i].state = MeteorState::DECAYING;
          meteors[i].speedX = 0;
          meteors[i].speedY = 0;

          // Efek getar pada kamera diakibatkan tabrakan
          shakeDuration = 0.5f;

          // Cek jika meteor menabrak pemain
          if (CheckCollisionCircleRec(meteors[i].center, meteors[i].radius, player.playerCollisionRect))
          {
            player.TriggerDamage(1);
          }
        }
      }

      // Jika meteor menabrak player, kurangi health pemain

      // Efek fade & scale sebelum benar-benar dihancurkan
      if (meteors[i].state == MeteorState::DECAYING)
      {
        int fadeSubtraction = 10;
        if (meteors[i].color.a > fadeSubtraction)
        {
          meteors[i].color.a -= fadeSubtraction;
          meteors[i].scale += 0.01f;
        }
        else
        {
          meteors[i].state = MeteorState::DESTROYING;
        }
      }
      // Menghancurkan semua meteor jika telah memenuhi persyaratan state
      if (meteors[i].y > screenHeight + meteors[i].meteorTexture.height || meteors[i].state == MeteorState::DESTROYING)
      {
        meteors.erase(meteors.begin() + i);
        i--;
      }
    }

    // Sinkronisasi frameData
    frameData["health"] = player.health;
    frameData["score"] = 0;

    // Logic camera shake effect

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

    // Menggambar GameUI
    gameUI.Draw(stateChange, frameData);

    EndDrawing();
  }
  CloseWindow();
}