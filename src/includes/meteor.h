#pragma once
#include "raylib.h"
#include <vector>

enum class MeteorState
{
  FALLING,
  DECAYING,
  DESTROYING
};

class Meteor
{
public:
  Meteor();
  void Update();
  void Draw() const;

  float x, y, radius, scale, speedX, speedY, rotation, TimeDecay;
  MeteorState state = MeteorState::FALLING;

  Vector2 center;
  std::vector<Vector2> trail;

  static Texture2D meteorTexture;
  Color color;

private:
  int max_trail_length;
};