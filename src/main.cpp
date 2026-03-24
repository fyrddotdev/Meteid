#if defined(PLATFORM_ANDROID)
#include <android_native_app_glue.h>
#endif

#include <iostream>

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "Global.h"

#include "meteor.h"
#include "bomber.h"
#include "player.h"
#include "playerExplosion.h"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include "gameUI.h"

// Initialisasi variabel diluar main() untuk effect camera shake
float shakeDuration;
float shakeIntensity = 4.0f;

int main(int argc, char *argv[]) // Entry point
{

  // Initialisasi GameWindow
  constexpr int screenWidth = 360;
  constexpr int screenHeight = 640;
  Vector2 screenCenter = {screenWidth / 2, screenHeight / 2};
  const Color backgroundColor = {35, 26, 63, 255};

  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(screenWidth, screenHeight, "METEID! Raylib edition");
  SetTextureFilter(GetFontDefault().texture, TEXTURE_FILTER_POINT);
  SetTargetFPS(60);

  // Initialisasi Camera
  Camera2D camera = {0};
  camera.offset = {screenWidth / 2, screenHeight / 2};
  camera.target = {screenWidth / 2, screenHeight / 2};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  // Initialisasi GameUI
  GameUI gameUI;
  gameUI.currState = GameUI::gameState::INGAME;
  GuiLoadStyle(ASSETS_PATH "styles/meteid!.rgs");

  // Initialisasi Player
  Player player;

  // Initialisasi Meteor
  std::vector<Meteor> meteors;

  Meteor::meteorTexture = LoadTexture(ASSETS_PATH "graphics/meteorid.png");
  Meteor meteor;
  float spawnTimer = 0.0f;
  float spawnInterval = GetRandomValue(3, 15) / 10.0f;

  // Inisialisasi Bomber
  Bomber bomber;

  // Inisialisasi effects
  PlayerExplosion playerExplosion;
  playerExplosion.Init();

  // Initialisasi Game Environtment
  const Rectangle PlayerLine = {-32, player.rect.y - 4, screenWidth + 64, 8};

  // Game loop Logic
  while (!WindowShouldClose())
  {

    // Inisialisasi paket di frame ini
    GameUI::UIPacket frameData;

    if (gameUI.currState == GameUI::gameState::INGAME || gameUI.currState == GameUI::gameState::GAMEOVER)
    {
      // Sistem spawn meteor dengan interval <SpawnInterval> detik
      spawnTimer += GetFrameTime();
      // std::cout << "Spawn timer :" << spawnTimer << std::endl;
      if (spawnTimer >= spawnInterval)
      {
        meteors.push_back(Meteor());
        spawnTimer = 0;
        spawnInterval = GetRandomValue(3, 15) / 10.0f;
        // std::cout << "Meteor Timer triggered" << std::endl;
      }

      // Update
      player.Update();
      bomber.Update();
      playerExplosion.Update();

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

            if (!player.isDead)
              player.score += 1;

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
      frameData["score"] = player.score;

      // Logic camera shake effect
      if (shakeDuration > 0)
      {
        shakeDuration -= GetFrameTime();
        // Efek guncang
        camera.offset.x = screenCenter.x + GetRandomValue(-shakeIntensity, shakeIntensity);
        camera.offset.y = screenCenter.y + GetRandomValue(-shakeIntensity, shakeIntensity);
      }
      else
      {
        // Kembali ke tengah dengan halus jika durasi habis
        camera.offset.x = Lerp(camera.offset.x, screenCenter.x, 0.1f);
        camera.offset.y = Lerp(camera.offset.y, screenCenter.y, 0.1f);

        // Paksa berhenti jika sudah sangat dekat dengan titik tengah
        if (CheckCollisionPointCircle(camera.offset, screenCenter, 0.1f))
        {
          camera.offset = screenCenter;
        }
      }
    }

    // Drawing logic
    BeginDrawing();

    ClearBackground(backgroundColor);

    BeginMode2D(camera);

    if (gameUI.currState == GameUI::gameState::INGAME || gameUI.currState == GameUI::gameState::GAMEOVER)
    {
      DrawRectangleRec(PlayerLine, WHITE);
      player.Draw();
      bomber.Draw();
      playerExplosion.Draw();
      for (auto &m : meteors)
        m.Draw();
    }
    EndMode2D();

    // Gambar UI di atas kamera
    gameUI.Draw(gameUI.currState, frameData);
    EndDrawing();
  }
  UnloadTexture(Meteor::meteorTexture);
  CloseWindow();
  return 0;
}