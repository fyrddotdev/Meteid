#pragma once
class Meteor
{
public:
  Meteor();
  void Update();
  void Draw() const;
  float x, y, radius, scale, speedX, speedY, rotation, TimeDecay;
  Vector2 center;
  std::vector<Vector2> trail;
  static Texture2D meteorTexture;
  Color color;

private:
  int max_trail_length;
};