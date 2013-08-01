#include <stdexcept>

#include <SDL2/SDL.h>

#include "client/Client.hpp"
#include "client/Interface.hpp"

const int DEFAULT_WIDTH = 800;
const int DEFAULT_HEIGHT = 600;
const int DEFAULT_DEPTH = 32;

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

    Uint16 windowWidth, windowHeight;

    SDL_Window *mainwindow; /* Our window handle */
    SDL_GLContext maincontext; /* Our opengl context handle */
    SDL_Event ev;
    SDL_DisplayMode displayMode;
    Uint32 now, last_frame_time;
    bool windowFocus = true;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) /* Initialize SDL's Video subsystem */
        throw std::runtime_error(SDL_GetError());

    SDL_GetDesktopDisplayMode(0, &displayMode);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    if (false)
    {
        windowWidth = displayMode.w;
        windowHeight = displayMode.h;

        mainwindow = SDL_CreateWindow("RMIT Portal", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,
                windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else
    {
        windowWidth = DEFAULT_WIDTH;
        windowHeight = DEFAULT_HEIGHT;

        mainwindow = SDL_CreateWindow("RMIT Portal", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,
                windowHeight, SDL_WINDOW_OPENGL);
    }

    if (nullptr == mainwindow)
        throw std::runtime_error(SDL_GetError());

    maincontext = SDL_GL_CreateContext(mainwindow);

    SDL_ShowCursor(0);
    //SDL_SetWindowGrab(mainwindow, SDL_TRUE);

    SDL_Log("Platform: %s", SDL_GetPlatform());
    SDL_Log("CPU cores: %d", SDL_GetCPUCount());
    SDL_Log("OpenGL version: %s", (char*) glGetString(GL_VERSION));
    SDL_Log("OpenGL renderer: %s", (char*) glGetString(GL_RENDERER));
    SDL_Log("Display resolution: %dx%d", displayMode.w, displayMode.h);

    client->initialize();
    client->reshape(windowWidth, windowHeight);

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

            case SDL_WINDOWEVENT:
                switch (ev.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    client->reshape(ev.window.data1, ev.window.data2);
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    windowFocus = false;
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    windowFocus = true;
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEMOTION:
                // Ugly hack to simulate mouse grabbing
                if (windowFocus)
                {
                    ev.motion.xrel = -(windowWidth / 2 - ev.motion.x);
                    ev.motion.yrel = -(windowHeight / 2 - ev.motion.y);

                }
                client->event(&ev);
                break;
            default:
                client->event(&ev);
                break;
            }
        }

        // Ugly hack to simulate mouse grabbing
        if (windowFocus)
            SDL_WarpMouseInWindow(mainwindow, windowWidth / 2, windowHeight / 2);


        /* Calculate time passed */
        now = SDL_GetTicks();
        client->update(now - last_frame_time);
        last_frame_time = now;

        /* Refresh display and flip buffers */
        client->interface->data.fps = fps;
        client->display(0);

        SDL_GL_SwapWindow(mainwindow);
        SDL_Delay(10);

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

    if (1 != client.use_count())
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Client ptr use count is not 1; cleanup is not working properly!");

    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(mainwindow);
    SDL_Quit();

    return EXIT_SUCCESS;
}
