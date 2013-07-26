#include "Portal.hpp"

#include <stdio.h>
#include <stdlib.h>

#include "Client.hpp"

const int DEFAULT_WIDTH = 800;
const int DEFAULT_HEIGHT = 600;
const int DEFAULT_DEPTH = 32;
const int DEFAULT_FLAGS = (SDL_OPENGL | SDL_RESIZABLE);

static SDL_Surface *screen;
static int videoFlags;

/* Frame counting */
static int frame_count;
static Uint32 frame_time;
static int quit_flag;
int fps;

void quit()
{
    quit_flag = 1;
}

int main(int argc, char **argv)
{
    std::shared_ptr<Client> client = std::make_shared<Client>(argc, argv);

    SDL_Event ev;
    Uint32 now, last_frame_time;

    quit_flag = 0;
    videoFlags = DEFAULT_FLAGS;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    screen = SDL_SetVideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_DEPTH, videoFlags);

    client->initialize();
    client->reshape(screen->w, screen->h);

    fps = 0;
    frame_count = 0;
    last_frame_time = frame_time = SDL_GetTicks();
    while (!quit_flag)
    {
        /* Process all pending events */
        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
            case SDL_QUIT:
                quit();
                break;
            case SDL_VIDEORESIZE:
                screen = SDL_SetVideoMode(ev.resize.w, ev.resize.h,
                DEFAULT_DEPTH, videoFlags);
                client->reshape(screen->w, screen->h);
                break;
            default:
                client->event(&ev);
                break;
            }
        }
        /* Calculate time passed */
        now = SDL_GetTicks();
        client->update(now - last_frame_time);
        last_frame_time = now;

        /* Refresh display and flip buffers */
        client->display(screen);
        SDL_GL_SwapBuffers();

        /* Update FPS */
        frame_count++;
        if (now - frame_time > 1000)
        {
            fps = (frame_count * 1000) / (now - frame_time);
            frame_count = 0;
            frame_time = now;
        }
    }

    client->cleanup();
    SDL_Quit();

    return EXIT_SUCCESS;
}
