#include <raylib.h>
#include <gameUI.h>
#include <vector>

int CalculatedBarWidth;

GameUI::GameUI()
{
  // Constructor
}

void GameUI::ingameUI(int health, int score) const
{
  // HealthBar UI

  CalculatedBarWidth = 37 * health;

  static std::vector<Texture2D> healthBarTexture = {
      LoadTexture("src/assets/graphics/health_bg.png"),
      LoadTexture("src/assets/graphics/health_fg.png"),
      LoadTexture("src/assets/graphics/health_heart.png")};

  NPatchInfo NPatchHealthbarFG = {
      {0, 0, 0, 32},
      4,
      0,
      4,
      0,
      NPATCH_THREE_PATCH_HORIZONTAL};

  DrawTextureEx(healthBarTexture[0], {32, 32}, 0, 0.6f, WHITE); // Draw healthbar bg
  DrawTextureNPatch(healthBarTexture[1], NPatchHealthbarFG, {35, 35, (float)CalculatedBarWidth, 32},
                    {0, 0}, 0, WHITE);                          // Draw healthbar fg
                                                                // DrawTextureEx(HealthbarTexture[1], {35, 35}, 0, 1.0f, WHITE);
  DrawTextureEx(healthBarTexture[2], {12, 24}, 0, 3.0f, WHITE); // Draw healthbar icon
}

void GameUI::Update()
{
  // Update logic
}

void GameUI::Draw(gameState stateChange, UIPacket &data) const
{

  switch (stateChange)
  {
  case gameState::MENU:
    DrawText("TEST STATE CHANGE", GetScreenWidth() / 2, GetScreenHeight() - 32, 14, WHITE);

    break;
  case gameState::INGAME:
    ingameUI(std::get<int>(data.at("health")), std::get<int>(data.at("score")));
    break;
  }
}