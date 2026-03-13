#pragma once

class Player
{
public:
  Player();
  void Update();
  void Draw() const;

  int speedX;
  Rectangle rect;

private:
  Vector2 origin;
  float rotation;
};