#include "Portal.hpp"

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

    SDL_Window *mainwindow; /* Our window handle */
    SDL_GLContext maincontext; /* Our opengl context handle */
    SDL_Event ev;
    SDL_DisplayMode displayMode;
    Uint32 now, last_frame_time;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) /* Initialize SDL's Video subsystem */
        throw std::runtime_error(SDL_GetError());

    SDL_GetDesktopDisplayMode(0, &displayMode);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    mainwindow = SDL_CreateWindow("RMIT Portal", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            displayMode.w, displayMode.h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_INPUT_GRABBED);

    if (nullptr == mainwindow)
      throw std::runtime_error(SDL_GetError());

    maincontext = SDL_GL_CreateContext(mainwindow);

    SDL_Log("OpenGL version: %s",(char*) glGetString(GL_VERSION));
    SDL_Log("OpenGL renderer: %s", (char*) glGetString(GL_RENDERER));
    SDL_Log("Display resolution: %dx%d", displayMode.w, displayMode.h);
    SDL_Log("Platform: %s", SDL_GetPlatform());
    SDL_Log("CPU cores: %d", SDL_GetCPUCount());

    client->initialize();
    client->reshape(displayMode.w, displayMode.h);

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
                switch (ev.window.type)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    client->reshape(ev.window.data1, ev.window.data2);
                    break;
                default:
                    break;
                }
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
        client->display(0, fps);
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

    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(mainwindow);
    SDL_Quit();

    return EXIT_SUCCESS;
}
