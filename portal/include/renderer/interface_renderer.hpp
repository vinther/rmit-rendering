#ifndef RENDERER_INTERFACE_RENDERER_HPP_
#define RENDERER_INTERFACE_RENDERER_HPP_

#include <string>
#include <memory>
#include <deque>
#include <tuple>
#include <chrono>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class client;

namespace assets
{
class Font;
}

typedef unsigned int GLuint;

class Interface
{
public:
    Interface();
    ~Interface();

    bool initialize(DataStore& dataStore);

	struct Data
	{
	    Uint16 fps;
	    Uint16 renderTime;

	    glm::vec3 cameraPosition, cameraDirection;
	} data;

	struct Settings
	{
	    Settings();

	    Uint16 width, height;
	    Uint16 ptSize;

	    std::string font;

	    bool showMessageConsole;
	    unsigned int consoleLine;
	} settings;

    struct RenderResults
    {
        RenderResults()
            : settings()
            , renderTime(0)
        {}

        Settings settings;
        std::chrono::microseconds renderTime;
    };

	void toggleConsole();

	void render(RenderResults& results);
	void update(Uint32 microseconds);
    void addMessage(const std::string& message);
    void addMessage(int category, SDL_LogPriority priority, const std::string& message);
    void scroll(int amount);
    void scrollTo(int line);

    struct Console
    {
        typedef std::tuple<std::string, SDL_Color> Message;

        Console();

        enum class State : unsigned char
        {
            HIDDEN = 0,
            TRANSITION_HIDE,
            TRANSITION_SHOW,
            SHOWN
        } state;

        enum class TransitionDirection : signed char
        {
            DOWN = -1,
            UP = 1
        } transitionDirection;

        float timeSinceStart;
        float consoleTransition;
        unsigned int linesPerPage;

        std::deque<Message> messages;
    } console;
private:
    Handle<assets::Font> fontAsset;

    SDL_Surface* surface;
    GLuint texture;

    std::chrono::high_resolution_clock::time_point startTime;

    void writeText(const std::string& str, const SDL_Color& color, float x, float y);
};

#endif /* RENDERER_INTERFACE_RENDERER_HPP_ */
