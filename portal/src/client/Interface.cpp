
/*
 * Interface.cpp
 *
 *  Created on: 29/07/2013
 *      Author: s3443408
 */


#include "client/Interface.hpp"

#include <sstream>

#include <GL/glut.h>

#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "assets/AssetManager.hpp"
#include "assets/Font.hpp"

#include "client/Client.hpp"

#include "Utilities.hpp"

Interface::Interface(std::shared_ptr<Client> client)
    : data()
    , settings({640, 480, "fonts/DroidSans.ttf", 14})
    , client(client)
    , initialized(false)
    , surface(nullptr)
    , texture(0)
{
}

bool Interface::initialize()
{
    if (-1 == TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL2 TTF library could not be initialized: %s", TTF_GetError());
        return false;
    }

    if (surface)
        SDL_FreeSurface(surface);

    font = client->assetManager->getOrCreate<assets::Font>(settings.font + std::to_string(settings.ptSize), settings.font, settings.ptSize);

    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    surface = SDL_CreateRGBSurface(0, settings.width, settings.height, 32, rmask, gmask, bmask, amask);

    if (!surface)
    {
        SDL_Log("SDL_CreateRGBSurface: %s", SDL_GetError());
        return false;
    }

    initialized = true;

    return true;
}

GLuint tex = 0;
void Interface::display(SDL_Renderer* sdlRenderer)
{
    UNUSED(sdlRenderer);

    if (!initialized)
        return;

    SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, 0, 0, 0, 1));

    std::stringstream sstream;

    const unsigned int fontHeight = TTF_FontHeight(font->font);

    sstream << "FPS: " << data.fps;
    writeText(sstream.str(), 20.0f, 1 * fontHeight + 4);

    sstream.str("");
    const auto& pos = data.cameraPosition;
    sstream << "Pos: <" << pos.x << ", " << pos.y << ", " << pos.z << ">";
    writeText(sstream.str(), 20.0f, 2 * fontHeight + 4);

    sstream.str("");
    const auto rot = data.cameraDirection;
    sstream << "Rot: <" << rot.x << ", " << rot.y << ", " << rot.z << ">";
    writeText(sstream.str(), 20.0f, 3 * fontHeight + 4);

    sstream.str("");
    sstream << "RenderTime: " << data.renderTime;
    writeText(sstream.str(), 20.0f, 4 * fontHeight + 4);

    if (!tex)
        glGenTextures(1, &tex);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(glm::ortho(0.0f,640.0f, 0.0f, 480.0f)));

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(1, 0); glVertex3f(settings.width, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(settings.width, settings.height, 0);
        glTexCoord2f(0, 1); glVertex3f(0, settings.height, 0);
    glEnd();

    glPopMatrix();

    glMatrixMode(GL_TEXTURE);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void Interface::writeText(const std::string& str, float x, float y)
{
    SDL_Color fgColor = {255, 255, 255, 0};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font->font, str.c_str(), fgColor);

    if (!textSurface)
        SDL_Log("TTF_RenderText_Solid: %s", SDL_GetError());

    SDL_Rect r = {(int) x, (surface->h) - (int) y, textSurface->w, textSurface->h};
    SDL_BlitSurface(textSurface, nullptr, surface, &r);

    SDL_FreeSurface(textSurface);
}

Interface::~Interface()
{
    SDL_FreeSurface(surface);
}
