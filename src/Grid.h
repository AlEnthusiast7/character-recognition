#pragma once
#include <vector>

class SDL_Renderer;

class Grid{
public:
  int blocks_wide;
  int blocks_tall;

  float block_width;
  float block_height;

  std::vector<int> pixels;

  Grid(int size_x, int size_y,int screen_width, int screen_height);
  void DrawGrid(SDL_Renderer *renderer);
  void DrawHighlight(SDL_Renderer *renderer);
  //helper function to draw black pixels
  void DrawBlack(SDL_Renderer *r, int row, int col);
  //registers a click 
  void DrawClick(SDL_Renderer *r, float x, float y);
  //clears the pixels array
  void ClearGrid();
};
