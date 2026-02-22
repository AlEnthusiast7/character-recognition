#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include "Grid.h"

#define screen_w 28*28
#define screen_h 28*28

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

Grid main_grid(28, 28, screen_w, screen_h);

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Character recognition", screen_w, screen_h, SDL_WINDOW_ALWAYS_ON_TOP, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, screen_w, screen_h, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    } 

    if (event->type == SDL_EVENT_KEY_DOWN)
      if (event->key.key == SDLK_C)
        main_grid.ClearGrid();

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColorFloat(renderer, 1.0f,1.0f,1.0f, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */
    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);


    main_grid.DrawGrid(renderer);

    float x,y;
    if (SDL_GetMouseState(&x, &y) & SDL_BUTTON_LMASK)
      main_grid.DrawClick(renderer, x, y);
    main_grid.DrawHighlight(renderer);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}


