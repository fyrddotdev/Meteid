#pragma once
#include <map>
#include <string>
#include <variant>
#include <raylib.h>

#include "guiGameOver.h"

class GameUI
{
public:
  enum class gameState
  {
    MENU,
    INGAME,
    GAMEOVER,
    ABOUT
  };
  gameState currState;
  using UIData = std::variant<int, float, std::string, bool>;
  using UIPacket = std::map<std::string, UIData>;

  // UI untuk gameState
  GameUI();

  // GUI Layout
  GuiGameOverState gameOverState;

  static float flashOpacity;
  static Color flashColor;
  static void FlashScreen(Color clr);
  void Update();
  static GameUI *Get() { return instance; }
  void Draw(gameState stateChange, UIPacket &data);

private:
  void ingameUI(int health, int score) const;
  static GameUI *instance;
};