#include <iostream>
#include <cmath>
#include <vector>
#include <raylib.h>
#include <raymath.h>

#include "meteor.h"
// Defining meteorTexture static member
Texture2D Meteor::meteorTexture = {0};

Meteor::Meteor()
    : x(0), y(-128), radius(0), scale(0.75f), speedX(0), speedY(0), rotation(0.0f), center{0, 0}, color(WHITE), max_trail_length(20)
{
  // if (meteorTexture.id == 0) {
  //     meteorTexture = LoadTexture("src/res/meteorid.png"); // Load meteorTexture if empty
  // }

  center.x = GetRandomValue(0, GetScreenWidth());
  rotation = GetRandomValue(0, 3600) / 10.0f;

  speedX = GetRandomValue(-20, 20) / 10.0f;
  speedY = GetRandomValue(50, 80) / 10.0f;

  // Calculate meteor radius
  // Performing basic diagonal math for hit detection
  float diagonal = (meteorTexture.width * scale / 2) * std::sqrt(2.0f);
  radius = (diagonal / 2);
}

void Meteor::Update()
{
  center.x += speedX;
  center.y += speedY;

  // Force x & y coordinates to the center of the texture
  x = center.x - (meteorTexture.width * scale / 2);
  y = center.y - (meteorTexture.height * scale / 2);

  // Wrap around the screen if the meteor goes off-bounds
  center.x = Wrap(center.x, 0 - (meteorTexture.width * scale / 2), GetScreenWidth() + (meteorTexture.width * scale / 2));

  // Meteor trail effect logic
  trail.insert(trail.begin(), center); // Insert current position as the first element in the array
  if ((int)(trail.size()) > max_trail_length)
  {
    trail.pop_back(); // Remove oldest trail point if maximum length is reached
  }
}

void Meteor::Draw() const
{
  // Trail rendering logic
  for (int i = 0; i < (int)(trail.size()); i++)
  {
    float aplhaPercentage = color.a / 255.0f; // Calculate alpha ratio based on meteor's current color
    float ratio = (float)(max_trail_length - i) / max_trail_length;
    Color color = Fade({226, 142, 114, 255}, ratio * aplhaPercentage);

    if (speedX == 0 || speedY == 0)
    {
      color = Fade({226, 142, 114, 255}, ratio * aplhaPercentage / 5);
    }
    DrawCircleV(trail[i], radius * ratio, color);
  }

  // Adjust texture origin for proper rotation and scaling
  Rectangle source = {
      0, 0, (float)meteorTexture.width, (float)meteorTexture.height};
  Rectangle dest = {
      center.x, center.y, (float)meteorTexture.width * scale, (float)meteorTexture.height * scale};

  DrawTexturePro(meteorTexture, source, dest, {dest.width / 2, dest.height / 2}, rotation, color);

  // Debugging purpose only!
  // DrawCircle(source.width / 2, source.height / 2, radius / 2, WHITE);
}