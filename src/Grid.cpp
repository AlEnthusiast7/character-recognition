#include <SDL3/SDL.h>
#include <iostream>
#include <math.h>
#include "Grid.h"

Grid::Grid(int size_x, int size_y, int screen_width, int screen_height) : 
  blocks_wide(size_x),
  blocks_tall(size_y),
  pixels(size_y, size_x)
{
  block_width = (float)screen_width/ (float) blocks_wide; 
  block_height = (float)screen_height/ (float) blocks_tall;
}

void Grid::DrawClick(SDL_Renderer *renderer, float x, float y)
{
  //how many blocks across and down is the cursor on
  int col = x/(float)(block_width);
  int row = y/(float)(block_height);

  // 1 is black
  if ( (row*blocks_wide) + col < blocks_wide*blocks_tall)
    // diffuse? effect, rather than solid block
    pixels(row, col) = pixels(row, col) < 255? pixels(row, col) + 3 : pixels(row, col);
}

void Grid::DrawGrid(SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColorFloat(renderer, 0.0f,0.0f,0.0f, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

  //draws vertical lines
  double x;
  for (int i = 0; i< blocks_wide+1; i++)
  {
    x = (block_width) * i;
    SDL_RenderLine(renderer, x, 0, x, block_width*blocks_wide);
  }

  //draws horizontal lines
  double y;
  for (int j = 0; j< blocks_tall+1; j++)
  {
    y = (block_height) * j;
    SDL_RenderLine(renderer, 0, y, block_height*blocks_tall, y);
  }

  //colors the grid
  for (int i = 0; i < blocks_tall; i++)
  {
    for(int j = 0; j< blocks_wide; j++){
      if (pixels(i, j))
        DrawBlack(renderer, i , j);
    }
  }
}

void Grid::ClearGrid()
{
  // resets grid to 0s
  pixels.zero();
}

void Grid::DrawBlack(SDL_Renderer *renderer, int row, int col)
{
  // sets renderer color to correct shade
  int shade = 255 - pixels(row, col);
  SDL_SetRenderDrawColor(renderer, shade, shade , shade, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */
  
  // x, y, width, height
  SDL_FRect square = {(float)col*block_width, (float)row*block_height, block_width, block_height};

  // draws the rect
  SDL_RenderFillRect(renderer, &square);
}

void Grid::DrawHighlight(SDL_Renderer *renderer, float mpos_x, float mpos_y)
{
  float x = ((int)((float)mpos_x/(float)block_width)) * block_width;
  float y = ((int)(mpos_y/block_height)) * block_height;

  SDL_FRect highlight = {x, y, block_width, block_height};

  SDL_SetRenderDrawColorFloat(renderer, 1.0f,0.0f,0.0f, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */
  SDL_RenderRect(renderer, &highlight);
}

