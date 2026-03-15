#include <iostream>
#include <cmath>
#include <vector>
#include <raylib.h>

#include "meteor.h"
// Mendefinisikan meteorTexture
Texture2D Meteor::meteorTexture = {0};

Meteor::Meteor()
    : x(0), y(-128), radius(0), scale(0.75f), speedX(0), speedY(0), rotation(0.0f), center{0, 0}, color(WHITE), max_trail_length(20)
{
  // if (meteorTexture.id == 0) {
  //     meteorTexture = LoadTexture("src/res/meteorid.png"); //Load meteorTexture jika kosong
  // }

  center.x = GetRandomValue(0, GetScreenWidth());
  rotation = GetRandomValue(0, 3600) / 10.0f;

  speedX = GetRandomValue(-20, 20) / 10.0f;
  speedY = GetRandomValue(50, 80) / 10.0f;

  // Mengkalkulasi radius meteor
  // Doing some basic math below ( its easy asf )
  float diagonal = (meteorTexture.width * scale / 2) * std::sqrt(2.0f);
  radius = (diagonal / 2);
}

void Meteor::Update()
{
  center.x += speedX;
  center.y += speedY;

  // Memaksa x & y ditengah-tengah texture
  x = center.x - (meteorTexture.width * scale / 2);
  y = center.y - (meteorTexture.height * scale / 2);

  // Teleport jika keluar layar
  if (center.x > (GetScreenWidth() + (meteorTexture.width)))
  {
    center.x = 0 - meteorTexture.width + 1;
  }
  else if (center.x < (0 - (meteorTexture.width)))
  {
    center.x = GetScreenWidth();
  }

  // Trail effect untuk meteor
  trail.insert(trail.begin(), center); // Memasukan posisi meteor sekarang pada array pertama
  if ((int)(trail.size()) > max_trail_length)
  {
    trail.pop_back(); // Menghapus trail lama jika jumlah trail sudah mencapai maksimum
  }

  // Decay sebelum meteor didestroy
}

void Meteor::Draw() const
{
  // Spawn trail logic
  for (int i = 0; i < (int)(trail.size()); i++)
  {
    float aplhaPercentage = color.a / 255.0f; // Mencari rasio persentase dari warna meteor
    float ratio = (float)(max_trail_length - i) / max_trail_length;
    Color color = Fade({226, 142, 114, 255}, ratio * aplhaPercentage);

    if (speedX == 0 || speedY == 0)
    {
      color = Fade({226, 142, 114, 255}, ratio * aplhaPercentage / 5);
    }
    DrawCircleV(trail[i], radius * ratio, color);
  }

  // Code dibawah ini memperbaiki origin pada texture
  Rectangle source = {
      0, 0, (float)meteorTexture.width, (float)meteorTexture.height};
  Rectangle dest = {
      center.x, center.y, (float)meteorTexture.width * scale, (float)meteorTexture.height * scale};

  DrawTexturePro(meteorTexture, source, dest, {dest.width / 2, dest.height / 2}, rotation, color);

  // Debugging purpose only!
  // DrawCircle(source.width / 2, source.height / 2, radius / 2, WHITE);
}