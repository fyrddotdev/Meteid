#if defined(PLATFORM_ANDROID)
#include <android_native_app_glue.h>
#endif

#include <iostream>
#include <string>

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

int main() // Entry point
{
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);

  RenderTexture2D targetMobile = {0};
  if (IS_DESKTOP)
  {
    // InitWindows first so that OpenGL Context can detect monitor's
    InitWindow(10, 10, "Loading...");

    int mHeight = GetMonitorHeight(0);
    int winH = (int)(mHeight * 0.875f);
    int winW = (int)(winH * (9.0f / 16.0f));

    // Then set the resolution
    SetWindowSize(winW, winH);
    SetWindowTitle("METEID! Raylib Edition");

    // set the window to the center of the monitor screen
    SetWindowPosition((GetMonitorWidth(0) - winW) / 2, (GetMonitorHeight(0) - winH) / 2);
  }
  else
  {
    // In android, force to init with default base resolution
    InitWindow(BASE_WIDTH, BASE_HEIGHT, "METEID!");
    targetMobile = LoadRenderTexture(BASE_WIDTH, BASE_HEIGHT);
    SetTextureFilter(targetMobile.texture, TEXTURE_FILTER_POINT);
  }

  // We need to initialize this both variables so that the code can run without any error missing variables
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();
  Vector2 screenCenter = {(float)screenWidth / 2, (float)screenHeight / 2};

  const Color backgroundColor = {35, 26, 63, 255};

  SetTextureFilter(GetFontDefault().texture, TEXTURE_FILTER_POINT);
  SetTargetFPS(60);

  // Initialize Camera
  Camera2D camera = {0};

  camera.offset = {(float)screenWidth / 2, (float)screenHeight / 2};
  camera.target = {(float)screenWidth / 2, (float)screenHeight / 2};

  // Paksa target dan offset kamera jadi angka bulat
  camera.target.x = floorf(camera.target.x);
  camera.target.y = floorf(camera.target.y);
  camera.offset.x = floorf(camera.offset.x);
  camera.offset.y = floorf(camera.offset.y);

  camera.zoom = 1.0f;

  // Initialize CameraUI
  Camera2D cameraUI = {0};
  cameraUI.zoom = 1.0f;

  // Initialize GameUI
  GameUI gameUI;
  gameUI.currState = GameUI::gameState::INGAME;
  GuiLoadStyle(ASSETS_PATH "styles/meteid.rgs");

  // Initialize Player
  Player player;

  // Initialize Meteor
  std::vector<Meteor> meteors;

  Meteor::meteorTexture = LoadTexturePixel(ASSETS_PATH "graphics/meteorid.png");
  Meteor meteor;
  float spawnTimer = 0.0f;
  float spawnInterval = GetRandomValue(3, 15) / 10.0f;

  // Initialize Bomber
  Bomber bomber;

  // Initialize effects
  PlayerExplosion playerExplosion;
  playerExplosion.Init();

  // Initialize Game Environment
  const Rectangle PlayerLine = {-32, player.rect.y - 4, (float)screenWidth + 64, 8};

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
    if (!IS_DESKTOP)
    {
      BeginTextureMode(targetMobile);
      ClearBackground(backgroundColor);

      DrawText("test", 50, 53, 16, GREEN);

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

      // UI
      BeginMode2D(cameraUI);

      // Draw UI on top of the camera space
      gameUI.Draw(gameUI.currState, frameData);

      EndMode2D();
      EndTextureMode();

      BeginDrawing();
      // Source rectangle is the entire virtual screen texture
      Rectangle sourceRec = {0.0f, 0.0f, (float)targetMobile.texture.width, (float)-targetMobile.texture.height};
      // Destination rectangle is the entire physical screen
      Rectangle destRec = {0.0f, 0.0f, (float)screenWidth, (float)screenHeight};
      // Draw the texture, scaled to fit the screen
      DrawTexturePro(targetMobile.texture, sourceRec, destRec, {0, 0}, 0.0f, WHITE);
      EndDrawing();
    }
    else
    {
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

      // UI
      BeginMode2D(cameraUI);

      // Draw UI on top of the camera space
      gameUI.Draw(gameUI.currState, frameData);

      EndMode2D();

      EndDrawing();
    }
  }
  UnloadTexture(Meteor::meteorTexture);
  CloseWindow();
  return 0;
}