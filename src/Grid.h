#pragma once
#include "Math/Matrix.h"
#include <vector>

class SDL_Renderer;

class Grid {
public:
  int blocks_wide;
  int blocks_tall;

  float block_width;
  float block_height;

  Matrix pixels;

  Grid(int size_x, int size_y, int screen_width, int screen_height);

  void DrawGrid(SDL_Renderer *renderer);
  // highlights square under mouse
  void DrawHighlight(SDL_Renderer *renderer, float mpos_x, float mpos_y);
  // helper function to draw black pixels
  void DrawBlack(SDL_Renderer *r, int row, int col);
  // registers a click
  void DrawClick(SDL_Renderer *r, float x, float y);

  Matrix get_flattened_grid() { return pixels.flatten(); };
  // clears the pixels array
  void ClearGrid();
};
