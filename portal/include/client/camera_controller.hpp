#ifndef CLIENT_CAMERA_CONTROLLER_HPP_
#define CLIENT_CAMERA_CONTROLLER_HPP_

#include <memory>

#include <SDL2/SDL.h>

class client;

class camera_controller
{
public:
    camera_controller(std::shared_ptr<client> client);
    virtual ~camera_controller();

    void on_key_down(SDL_Keycode key, Uint16 mod);
    void on_key_up(SDL_Keycode key, Uint16 mod);
    void on_mouse_down(Uint8 button, Uint16 x, Uint16 y);
    void on_mouse_up(Uint8 button, Uint16 x, Uint16 y);
    void on_mouse_move(Uint16 x, Uint16 y, Sint16 xrel, Sint16 yrel);

    void update(Uint32 ms);

    struct State
    {
        State();

        bool move_forward;
        bool move_backward;
        bool move_left;
        bool move_right;
        bool move_up;
        bool move_down;

        float move_speed;
    } state;
private:
    std::shared_ptr<client> client;
};

#endif /* CLIENT_CAMERA_CONTROLLER_HPP_ */
