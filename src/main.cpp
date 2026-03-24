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

// Initialize variables outside main() for camera shake effect
float shakeDuration;
float shakeIntensity = 4.0f;

int main(int argc, char *argv[]) // Entry point
{

  // Initialize GameWindow
  constexpr int screenWidth = 360;
  constexpr int screenHeight = 640;
  Vector2 screenCenter = {screenWidth / 2, screenHeight / 2};
  const Color backgroundColor = {35, 26, 63, 255};

  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(screenWidth, screenHeight, "METEID! Raylib edition");
  SetTextureFilter(GetFontDefault().texture, TEXTURE_FILTER_POINT);
  SetTargetFPS(60);

  // Initialize Camera
  Camera2D camera = {0};
  camera.offset = {screenWidth / 2, screenHeight / 2};
  camera.target = {screenWidth / 2, screenHeight / 2};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  // Initialize GameUI
  GameUI gameUI;
  gameUI.currState = GameUI::gameState::INGAME;
  GuiLoadStyle(ASSETS_PATH "styles/meteid!.rgs");

  // Initialize Player
  Player player;

  // Initialize Meteor
  std::vector<Meteor> meteors;

  Meteor::meteorTexture = LoadTexture(ASSETS_PATH "graphics/meteorid.png");
  Meteor meteor;
  float spawnTimer = 0.0f;
  float spawnInterval = GetRandomValue(3, 15) / 10.0f;

  // Initialize Bomber
  Bomber bomber;

  // Initialize effects
  PlayerExplosion playerExplosion;
  playerExplosion.Init();

  // Initialize Game Environment
  const Rectangle PlayerLine = {-32, player.rect.y - 4, screenWidth + 64, 8};

  // Game Loop Logic
  while (!WindowShouldClose())
  {

    // Initialize packet for this frame
    GameUI::UIPacket frameData;

    if (gameUI.currState == GameUI::gameState::INGAME || gameUI.currState == GameUI::gameState::GAMEOVER)
    {
      // Meteor spawning system with <SpawnInterval> second interval
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

      // Loop to check and update each meteor[i] within the meteors container
      for (size_t i = 0; i < meteors.size(); i++)
      {
        meteors[i].Update();

        if (meteors[i].state == MeteorState::FALLING) // Check if meteor is in FALLING state
        {

          // If meteor hits the player's boundary line, change state to DECAYING
          if (CheckCollisionCircleRec(meteors[i].center, meteors[i].radius, PlayerLine))
          {
            meteors[i].state = MeteorState::DECAYING;
            meteors[i].speedX = 0;
            meteors[i].speedY = 0;

            if (!player.isDead)
              player.score += 1;

            // Camera shake effect triggered by collision
            shakeDuration = 0.5f;

            // Check if meteor hits the player
            if (CheckCollisionCircleRec(meteors[i].center, meteors[i].radius, player.playerCollisionRect))
            {
              player.TriggerDamage(1);
            }
          }
        }

        // Fade & scale effect before being fully destroyed
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

        // Destroy all meteors that meet destruction requirements
        if (meteors[i].y > screenHeight + meteors[i].meteorTexture.height || meteors[i].state == MeteorState::DESTROYING)
        {
          meteors.erase(meteors.begin() + i);
          i--;
        }
      }

      // Synchronize frameData
      frameData["health"] = player.health;
      frameData["score"] = player.score;

      // Camera shake effect logic
      if (shakeDuration > 0)
      {
        shakeDuration -= GetFrameTime();
        // Applying shake offset
        camera.offset.x = screenCenter.x + GetRandomValue(-shakeIntensity, shakeIntensity);
        camera.offset.y = screenCenter.y + GetRandomValue(-shakeIntensity, shakeIntensity);
      }
      else
      {
        // Smoothly return to center when duration ends
        camera.offset.x = Lerp(camera.offset.x, screenCenter.x, 0.1f);
        camera.offset.y = Lerp(camera.offset.y, screenCenter.y, 0.1f);

        // Snap to center if very close to the target point
        if (CheckCollisionPointCircle(camera.offset, screenCenter, 0.1f))
        {
          camera.offset = screenCenter;
        }
      }
    }

    // Drawing Logic
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

    // Draw UI on top of the camera space
    gameUI.Draw(gameUI.currState, frameData);
    EndDrawing();
  }
  UnloadTexture(Meteor::meteorTexture);
  CloseWindow();
  return 0;
}