/* $Id: example.c 4 2006-07-03 12:40:56Z aholkner $ */

#include "sdl-base.h"

#include <stdio.h>

/* Called once at the beginning of the program, after SDL is initialised. */
void init()
{
}

/* Called every time the window is resized. */
void reshape(int width, int height)
{
}

/* Called once every frame. */
void display(SDL_Surface *screen)
{
    glClearColor(1, 0, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    printf("%5d FPS\n", fps);
    fflush(stdout);
}

/* Called every frame, with the number of milliseconds since the last frame. */
void update(int milliseconds)
{
}

/* Called every time an event needs to be processed. */
void event(SDL_Event *event)
{
}

/* Called before quit */
void cleanup()
{
}
