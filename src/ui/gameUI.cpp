#include <raylib.h>
#include <gameUI.h>
#include <vector>

float CalculatedBarWidth;

GameUI::GameUI()
{
  // Constructor
}

void GameUI::ingameUI(int health, int score) const
{
  // HealthBar UI
  CalculatedBarWidth = 12.3 * health;

  static std::vector<Texture2D> healthBarTexture = {
      LoadTexture("src/assets/graphics/health_bg.png"),
      LoadTexture("src/assets/graphics/health_fg.png"),
      LoadTexture("src/assets/graphics/health_heart.png")};

  // Sejujurnya, kode ini sangat susah untuk dimaintenance. Keubah dikit kacau healtbarnya jir
  NPatchInfo NPatchHealthbarFG = {
      {0, 3, 0, 27},
      4,
      0,
      4,
      0,
      NPATCH_THREE_PATCH_HORIZONTAL};

  DrawTextureEx(healthBarTexture[0], {16, 16}, 0, 0.5f, WHITE); // Draw healthbar bg
  DrawTextureNPatch(healthBarTexture[1], NPatchHealthbarFG, {19, 19, CalculatedBarWidth, 0},
                    {0, 0}, 0, WHITE);                         // Draw healthbar fg
                                                               // DrawTextureEx(HealthbarTexture[1], {35, 35}, 0, 1.0f, WHITE);
  DrawTextureEx(healthBarTexture[2], {4, 10}, 0, 2.5f, WHITE); // Draw healthbar icon
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