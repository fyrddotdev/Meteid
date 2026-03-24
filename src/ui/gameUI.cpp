#include <raylib.h>
#include <gameUI.h>
#include <vector>
#include "Global.h"

#include "player.h"

GameUI *GameUI::instance = nullptr;

#define GUI_GAMEOVER_IMPLEMENTATION
#include "guiGameOver.h"

float GameUI::flashOpacity = 0.0f;
Color GameUI::flashColor = WHITE;
float CalculatedBarWidth;

GameUI::GameUI()
{
  instance = this;
  gameOverState = InitGuiGameOver();
}

void GameUI::ingameUI(int health, int score) const
{
  // HealthBar UI calculations
  CalculatedBarWidth = 12.3 * health;

  static std::vector<Texture2D> healthBarTexture = {
      LoadTexture(ASSETS_PATH "graphics/health_bg.png"),
      LoadTexture(ASSETS_PATH "graphics/health_fg.png"),
      LoadTexture(ASSETS_PATH "graphics/health_heart.png")};

  // Honestly, this code is very hard to maintain.
  // Minor changes can easily break the health bar alignment.
  NPatchInfo NPatchHealthbarFG = {
      {0, 3, 0, 27},
      4,
      0,
      4,
      0,
      NPATCH_THREE_PATCH_HORIZONTAL};

  DrawTextureEx(healthBarTexture[0], {16, 16}, 0, 0.5f, WHITE); // Draw healthbar background
  DrawTextureNPatch(healthBarTexture[1], NPatchHealthbarFG, {19, 19, CalculatedBarWidth, 0},
                    {0, 0}, 0, WHITE); // Draw healthbar foreground

  DrawTextureEx(healthBarTexture[2], {4, 10}, 0, 2.5f, WHITE); // Draw healthbar icon
}

void GameUI::FlashScreen(Color clr)
{
  if (flashOpacity > 0.0f)
  {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(clr, flashOpacity));
    flashOpacity -= 0.05f;
  }
}

void GameUI::Update()
{
  // Update logic
}

void GameUI::Draw(gameState stateChange, UIPacket &data)
{
  if (flashOpacity > 0.0f)
  {
    FlashScreen(flashColor);
  }

  switch (stateChange)
  {
  case gameState::MENU:
    DrawText("TEST STATE CHANGE", GetScreenWidth() / 2, GetScreenHeight() - 32, 14, WHITE);

    break;
  case gameState::INGAME:
    ingameUI(std::get<int>(data.at("health")), std::get<int>(data.at("score")));
    break;
  case gameState::GAMEOVER:
    if (gameOverState.GameOverWBoxActive)
    {

      GuiSetStyle(0, TEXT_SIZE, 20);
      GuiPanel(gameOverState.layoutRecs[0], "YOU DEAD");

      GuiSetStyle(0, TEXT_SIZE, 16);
      int score = std::get<int>(data.at("score"));
      GuiLabel(gameOverState.layoutRecs[1], TextFormat("SCORE : %d", score));

      if (GuiButton(gameOverState.layoutRecs[2], "EXIT TO MAIN MENU"))
      {
        currState = gameState::MENU; // Switch game state back to menu
      }
    }
    break;
  }
}