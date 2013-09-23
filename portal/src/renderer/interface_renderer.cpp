
/*
 * Interface.cpp
 *
 *  Created on: 29/07/2013
 *      Author: s3443408
 */


#include "client/Interface.hpp"

#include <sstream>
#include <iomanip>

#include <GL/glut.h>

#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "assets/Font.hpp"

#include "client/Client.hpp"

#include "Utilities.hpp"

Interface::Interface()
    : data()
    , settings()
    , surface(nullptr)
    , texture(0)
    , startTime(std::chrono::high_resolution_clock::now())
{
    interface_console.state = console::State::HIDDEN;
}

Interface::~Interface()
{
    if (texture)
        glDeleteTextures(1, &texture);

    SDL_FreeSurface(surface);
}

Interface::settings_t::settings_t()
    : width(640)
    , height(480)
    , ptSize(14)
    , font("fonts/DroidSans.ttf")
    , showMessageConsole(false)
    , consoleLine(0)
{
}

Interface::console::console()
    : state(State::HIDDEN)
    , transitionDirection(TransitionDirection::DOWN)
    , timeSinceStart(0.0f)
    , consoleTransition(0.0f)
    , linesPerPage(16)
    , messages()
{
}

bool Interface::initialize(assets::asset_store& dataStore)
{
    if (-1 == TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL2 TTF library could not be initialized: %s", TTF_GetError());
        return false;
    }

    if (surface)
        SDL_FreeSurface(surface);

    if (texture)
        glDeleteTextures(1, &texture);

    fontAsset = dataStore.create<assets::Font>(settings.font + std::to_string(settings.ptSize), settings.font, settings.ptSize);

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

    if (!texture)
    {
        glGenTextures(1, &texture);
    }

    interface_console.linesPerPage = (0.666f * settings.height - 24.0f) / TTF_FontHeight(fontAsset->font) + 1;

    return true;
}

void Interface::update(Uint32 microseconds)
{
    if (console::State::TRANSITION_SHOW == interface_console.state || console::State::TRANSITION_HIDE == interface_console.state)
    {
        interface_console.timeSinceStart += -((float) interface_console.transitionDirection) * (microseconds / 200000.0f);

        if (interface_console.timeSinceStart < 0.0f)
            interface_console.state = console::State::HIDDEN;
        else if (interface_console.timeSinceStart > 1.0f)
            interface_console.state = console::State::SHOWN;
        else
            interface_console.consoleTransition = glm::smoothstep(0.0f, 1.0f, interface_console.timeSinceStart);
    }
}

void Interface::toggleConsole()
{
    switch (interface_console.state)
    {
    case console::State::SHOWN:
        interface_console.state = console::State::TRANSITION_HIDE;
        interface_console.timeSinceStart = 1.0f;
        interface_console.consoleTransition = 1.0f;
        interface_console.transitionDirection = console::TransitionDirection::UP;
        break;
    case console::State::HIDDEN:
        interface_console.state = console::State::TRANSITION_SHOW;
        interface_console.timeSinceStart = 0.0f;
        interface_console.consoleTransition = 0.0f;
        interface_console.transitionDirection = console::TransitionDirection::DOWN;
        break;
    case console::State::TRANSITION_HIDE:
        interface_console.state = console::State::TRANSITION_SHOW;
        interface_console.transitionDirection = console::TransitionDirection::DOWN;
        break;
    case console::State::TRANSITION_SHOW:
        interface_console.state = console::State::TRANSITION_HIDE;
        interface_console.transitionDirection = console::TransitionDirection::UP;
        break;
    }
}

inline Interface::console::Message buildMessage(
        unsigned int category,
        SDL_LogPriority priority,
        const std::string& message,
        std::chrono::microseconds time)
{
    UNUSED(category);

    const unsigned int seconds = std::chrono::duration_cast<std::chrono::seconds>(time).count();
    const unsigned int miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
    const unsigned int microseconds = std::chrono::duration_cast<std::chrono::microseconds>(time).count();

    SDL_Color color{255, 255, 255, 255};
    std::stringstream ss("");

    ss << std::setw( 3 ) << std::setfill('0') << seconds << ":";
    ss << std::setw( 3 ) << std::setfill('0') << miliseconds - seconds * 1000 << ":";
    ss << std::setw( 3 ) << std::setfill('0') << microseconds - miliseconds * 1000 << " ";

    switch (category)
    {
    case SDL_LOG_CATEGORY_APPLICATION: ss << "[APP] "; break;
    case client::PORTAL_LOG_CATEGORY_CLIENT: ss << "[CLIENT] "; break;
    case client::PORTAL_LOG_CATEGORY_ASSETS: ss << "[ASSETS] "; break;
    case client::PORTAL_LOG_CATEGORY_RENDERER: ss << "[RENDER] "; break;
    default: ss << "[UNKNOWN] "; break;
    }

    switch (priority)
    {
    case SDL_LOG_PRIORITY_INFO:
        ss << "[INFO] "; break;
    case SDL_LOG_PRIORITY_DEBUG:
        ss << "[DEBUG] "; break;
    case SDL_LOG_PRIORITY_ERROR:
        ss << "[ERROR] "; color = {255, 0, 0, 255}; break;
    case SDL_LOG_PRIORITY_VERBOSE:
        ss << "[VERBOSE] "; break;
    default:
        break;
    }

    ss << message;

    return Interface::console::Message{ss.str(), color};
}

void Interface::render(render_results& results)
{
    using namespace std::chrono;
    const auto timeBegin = high_resolution_clock::now();

    SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, 0, 0, 0, 1));

    std::stringstream sstream;

    const unsigned int fontHeight = TTF_FontHeight(fontAsset->font);
    const SDL_Color whiteColor{255, 255, 255, 255};

    sstream << "FPS: " << data.fps;
    writeText(sstream.str(), whiteColor, 24.0f, 1 * fontHeight + 24.0f);

    sstream.str("");
    const auto& pos = data.cameraPosition;
    sstream << "Pos: <" << pos.x << ", " << pos.y << ", " << pos.z << ">";
    writeText(sstream.str(), whiteColor, 24.0f, 2 * fontHeight + 24.0f);

    sstream.str("");
    const auto rot = data.cameraDirection;
    sstream << "Rot: <" << rot.x << ", " << rot.y << ", " << rot.z << ">";
    writeText(sstream.str(), whiteColor, 24.0f, 3 * fontHeight + 24.0f);

    sstream.str("");
    sstream << "RenderTime: " << data.renderTime;
    writeText(sstream.str(), whiteColor, 24.0f, 4 * fontHeight + 24.0f);

    if (console::State::HIDDEN != interface_console.state)
    {
        const float y = console::State::SHOWN == interface_console.state ? 1.0f : interface_console.consoleTransition;
        unsigned int messageIndex = 0;

        const auto begin = std::begin(interface_console.messages) + settings.consoleLine;
        const auto end = std::min(begin + interface_console.linesPerPage, std::end(interface_console.messages));

        for (auto it = begin; it < end; ++it)
        {
            const auto& message = *it;

            writeText(
                    std::get<0>(message),
                    std::get<1>(message),
                    24.0f,  settings.height - y * settings.height * 0.666f + messageIndex * fontHeight + 24.0f
                    );

            ++messageIndex;
        }
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(glm::ortho(0.0f, (float) settings.width, 0.0f, (float) settings.height)));

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    {
        glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
        glm::vec2 boxSize(5 * 64.0, 5 * fontHeight);
        glm::vec2 boxOffset(16.0f, 16.0f);

        glBegin(GL_QUADS);
            glVertex3f(boxOffset.x, boxOffset.y, 0);
            glVertex3f(boxOffset.x + boxSize.x, boxOffset.y, 0);
            glVertex3f(boxOffset.x + boxSize.x, boxOffset.y + boxSize.y, 0);
            glVertex3f(boxOffset.x, boxOffset.y + boxSize.y, 0);
        glEnd();
    }

    if (console::State::HIDDEN != interface_console.state)
    {
        const float y = console::State::SHOWN == interface_console.state ? 1.0f : interface_console.consoleTransition;

        glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
        glm::vec2 boxSize(settings.width, settings.height * 0.666f);
        glm::vec2 boxOffset(0.0f, settings.height - y * settings.height * 0.666f);

        glBegin(GL_QUADS);
            glVertex3f(boxOffset.x, boxOffset.y, 0);
            glVertex3f(boxOffset.x + boxSize.x, boxOffset.y, 0);
            glVertex3f(boxOffset.x + boxSize.x, boxOffset.y + boxSize.y, 0);
            glVertex3f(boxOffset.x, boxOffset.y + boxSize.y, 0);
        glEnd();
    }

    glPopMatrix();

    // Draw the text texture
    glBindTexture(GL_TEXTURE_2D, texture);

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(1, 0); glVertex3f(settings.width, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(settings.width, settings.height, 0);
        glTexCoord2f(0, 1); glVertex3f(0, settings.height, 0);
    glEnd();

    glPopMatrix();

    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    // End

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    results.settings = settings;
    results.renderTime = duration_cast<microseconds>(high_resolution_clock::now() - timeBegin);
}

void Interface::scroll(int amount)
{
    unsigned int& line = settings.consoleLine;

    if ((int) line + amount < 0)
        line = 0;
    else
        line += amount;

    line = glm::min(line, (unsigned int) interface_console.messages.size());
}

void Interface::scrollTo(int line)
{
    if (line < 0 && interface_console.messages.size() >= interface_console.linesPerPage)
        line = interface_console.messages.size() - interface_console.linesPerPage;

    settings.consoleLine = glm::clamp((unsigned int) line, 0u, (unsigned int) interface_console.messages.size());
}

void Interface::writeText(const std::string& str, const SDL_Color& color, float x, float y)
{
    SDL_Surface* textSurface = TTF_RenderText_Blended(fontAsset->font, str.c_str(), color);

    if (nullptr == textSurface)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_RenderText_Solid: %s", SDL_GetError());
    else
    {
        SDL_Rect r = {(int) x, (surface->h) - (int) y, textSurface->w, textSurface->h};
        SDL_BlitSurface(textSurface, nullptr, surface, &r);

        SDL_FreeSurface(textSurface);
    }
}

void Interface::addMessage(int category, SDL_LogPriority priority, const std::string& message)
{
    interface_console.messages.push_front(
            buildMessage(category, priority, message,
            std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime))
            );

    if (0 != settings.consoleLine)
        settings.consoleLine += 1;

    if (interface_console.messages.size() >= 4096 * 2)
        interface_console.messages.resize(4096);
}

void Interface::addMessage(const std::string& message)
{
    addMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, message);
}

