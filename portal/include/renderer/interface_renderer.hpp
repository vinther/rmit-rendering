#ifndef RENDERER_INTERFACE_RENDERER_HPP_
#define RENDERER_INTERFACE_RENDERER_HPP_

#include <string>
#include <memory>
#include <deque>
#include <tuple>
#include <chrono>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include "assets/asset_store.hpp"
#include "assets/font.hpp"

typedef unsigned int GLuint;

class interface_renderer
{
public:
    interface_renderer();
    ~interface_renderer();

    bool initialize(assets::asset_store& dataStore);

	struct Data
	{
	    Uint16 fps;
	    Uint16 renderTime;

	    glm::vec3 cameraPosition, cameraDirection;
	} data;

	struct settings_t
	{
	    settings_t();

	    Uint16 width, height;
	    Uint16 ptSize;

	    std::string font;

	    bool showMessageConsole;
	    unsigned int consoleLine;
	} settings;

    struct render_results
    {
        render_results()
            : settings()
            , renderTime(0)
        {}

        settings_t settings;
        std::chrono::microseconds renderTime;
    };

	void toggleConsole();

	void render(render_results& results);
	void update(Uint32 microseconds);
    void addMessage(const std::string& message);
    void addMessage(int category, SDL_LogPriority priority, const std::string& message);
    void scroll(int amount);
    void scrollTo(int line);

    struct console
    {
        typedef std::tuple<std::string, SDL_Color> Message;

        console();

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
    } interface_console;
private:
    handle<assets::font> fontAsset;

    SDL_Surface* surface;
    GLuint texture;

    std::chrono::high_resolution_clock::time_point startTime;

    void writeText(const std::string& str, const SDL_Color& color, float x, float y);
};

#endif /* RENDERER_INTERFACE_RENDERER_HPP_ */
