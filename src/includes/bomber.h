#pragma once
#include <raylib.h>
#include <vector>

class Bomber
{
public:
  Bomber();
  void Update();
  void Draw() const;

  struct Bomb
  {
    Texture texture;
    Vector2 spawnPosition;
    float maxSpeed;
    float acceleration;
  };
  std::vector<Bomb> bombs;

private:
  float size = 64.0f;
  float speedX = 3.0f;
  float x;

  static std::vector<Texture2D> bomberTextures;
};
