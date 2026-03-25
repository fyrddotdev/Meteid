#ifndef GLOBAL_H
#define GLOBAL_H

// Asset Path & Platform
#if defined(PLATFORM_ANDROID) || defined(__ANDROID__)
#define ASSETS_PATH ""
#define IS_DESKTOP false
#else
#define ASSETS_PATH "assets/"
#define IS_DESKTOP true
#endif

// Base Target Resolution (9:19)
constexpr int BASE_WIDTH = 360;
constexpr int BASE_HEIGHT = 760;

// LoadTexture but with texture filter point
inline Texture2D LoadTexturePixel(const char *fileName)
{
  Texture2D tex = LoadTexture(fileName);
  SetTextureFilter(tex, TEXTURE_FILTER_POINT);
  return tex;
}
#endif