#include <stdexcept>

#include <SDL2/SDL.h>

#include "assets/AssetManager.hpp"

#include "client/Client.hpp"
#include "client/Interface.hpp"
#include "client/CameraController.hpp"

#include "input/KeyboardHandler.hpp"
#include "input/MouseHandler.hpp"

#include "scene/Scene.hpp"

#include "renderer/Renderer.hpp"
#include "renderer/DebugRenderer.hpp"

#include "threading/ThreadPool.hpp"

#include "Utilities.hpp"
#include "Config.hpp"

const int DEFAULT_WIDTH = 1024;
const int DEFAULT_HEIGHT = 768;
const int DEFAULT_DEPTH = 32;

static int exitConditions;

void quit()
{
    exitConditions = 1;
}

int main(int argc, char **argv)
{
    Uint16 windowWidth, windowHeight;

    SDL_Window *window;
    SDL_Renderer* renderer;
    SDL_GLContext glContext;

    SDL_Event ev;
    SDL_DisplayMode displayMode;

    bool windowFocus = true;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
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

        window = SDL_CreateWindow("RMIT Portal", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,
                windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else
    {
        windowWidth = DEFAULT_WIDTH;
        windowHeight = DEFAULT_HEIGHT;

        window = SDL_CreateWindow("RMIT Portal", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,
                windowHeight, SDL_WINDOW_OPENGL);
    }

    if (nullptr == window)
        throw std::runtime_error(SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (nullptr == renderer)
        throw std::runtime_error(SDL_GetError());

    glContext = SDL_GL_CreateContext(window);

    SDL_ShowCursor(0);
    SDL_SetWindowGrab(window, SDL_FALSE);
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

    std::shared_ptr<Client> client = std::make_shared<Client>(argc, argv);
    client->initialize(window, glContext);
    client->reshape(windowWidth, windowHeight);

    SDL_Log("Build %d (%s)", config::build::buildNumber(), config::build::type());
    SDL_Log("Platform: %s (%d logical CPU cores)", SDL_GetPlatform(), SDL_GetCPUCount());
    SDL_Log("OpenGL: %s (%s)", (char*) glGetString(GL_VERSION), (char*) glGetString(GL_RENDERER));

    bool forceNoFocus = false;

    while (!exitConditions)
    {
        /* Process all pending events */
        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
            case SDL_QUIT:
                SDL_Log("Quit event received");
                quit();
                break;
            case SDL_KEYDOWN:
                if (ev.key.keysym.sym == 'g')
                {
                    forceNoFocus = !forceNoFocus;
                }

                client->event(&ev);
                break;
            case SDL_WINDOWEVENT:
                switch (ev.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    client->reshape(ev.window.data1, ev.window.data2);
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    windowFocus = false;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Window lost focus");
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    windowFocus = true;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Window gained focus");
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEMOTION:
                if (forceNoFocus)
                    break;

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
        if (windowFocus && !forceNoFocus)
            SDL_WarpMouseInWindow(window, windowWidth / 2, windowHeight / 2);

        client->prepareFrame(window, glContext);
        client->finalizeFrame(window, glContext);

        SDL_GL_SwapWindow(window);
    }

    client->cleanup();

    if (1 != client.use_count())
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Client ptr use count is not 1; cleanup is not working properly!");

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}