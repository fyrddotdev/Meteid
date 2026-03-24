#pragma once
#include <raylib.h>
#include <vector>

class PlayerExplosion
{
private:
  inline static PlayerExplosion *instance = nullptr;
  static std::vector<Texture2D> textures;
  float fadeValue;
  float rotation;
  float scale;

public:
  PlayerExplosion();
  void Init();
  void Update();
  void Draw() const;
  static PlayerExplosion *Get() { return instance; }

  bool enabled;
  bool isComplete;
};
