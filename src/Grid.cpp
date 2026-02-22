#include <SDL3/SDL.h>
#include <iostream>
#include "Grid.h"

Grid::Grid(int size_x, int size_y, int screen_width, int screen_height) : 
  blocks_wide(size_x),
  blocks_tall(size_y),
  pixels(size_x*size_y)
{
  block_width = (float)screen_width/ (float) blocks_wide; 
  block_height = (float)screen_height/ (float) blocks_tall;
}

void Grid::DrawClick(SDL_Renderer *renderer, float x, float y)
{
  int col = x/(float)(blocks_wide);
  int row = y/(float)(blocks_tall);
  pixels[(row*blocks_wide) + col] =  1;
}

void Grid::DrawGrid(SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColorFloat(renderer, 0.0f,0.0f,0.0f, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

  double x;
  for (int i = 0; i< blocks_wide+1; i++)
  {
    x = (block_width) * i;
    SDL_RenderLine(renderer, x, 0, x, block_width*blocks_wide);
  }

  double y;
  for (int j = 0; j< blocks_tall+1; j++)
  {
    y = (block_height) * j;
    SDL_RenderLine(renderer, 0, y, block_height*blocks_tall, y);
  }

  for (int i = 0; i < blocks_tall; i++)
  {
    for(int j = 0; j< blocks_wide; j++){
      if (pixels[(i*blocks_wide) + j])
        DrawBlack(renderer, i , j);
    }
  }

}

void Grid::DrawBlack(SDL_Renderer *renderer, int row, int col)
{
  SDL_FRect square = {(float)col*block_width, (float)row*block_height,
                      block_width, block_height};
  SDL_RenderFillRect(renderer, &square);
}

void Grid::DrawHighlight(SDL_Renderer *renderer)
{
  float window_x, window_y;
  SDL_GetMouseState(&window_x, &window_y);
  float render_x, render_y;
  SDL_RenderCoordinatesFromWindow(renderer, window_x, window_y, &render_x, &render_y);

  float x = ((int)((float)render_x/(float)block_width)) * block_width;
  float y = ((int)(render_y/block_height)) * block_height;

  SDL_FRect highlight = {x, y, block_width, block_height};

  SDL_SetRenderDrawColorFloat(renderer, 1.0f,0.0f,0.0f, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */
  SDL_RenderRect(renderer, &highlight);
}

