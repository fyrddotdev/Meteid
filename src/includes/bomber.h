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
    bool isActive;
    bool isHit;
    Texture texture;
    Vector2 spawnPosition;
    float maxSpeed;
    float speed;
    float acceleration;
    float rotation;
  };

private:
  float size = 64.0f;
  float speedX = 1.0f;
  float x;
  float y;

  static std::vector<Texture2D> bomberTextures;
};
