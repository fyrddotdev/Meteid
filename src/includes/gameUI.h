#pragma once
#include <map>
#include <string>
#include <variant>
#include <raylib.h>

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

  using UIData = std::variant<int, float, std::string, bool>;
  using UIPacket = std::map<std::string, UIData>;

  GameUI();
  void Update();
  void Draw(gameState stateChange, UIPacket &data) const;

private:
  void ingameUI(int health, int score) const;
};