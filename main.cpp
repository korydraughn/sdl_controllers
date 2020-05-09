#include <SDL.h>

#include <cmath>
#include <memory>
#include <iostream>

// Tests SDL controllers. In particular, the official Nintendo USB 
// GameCube Adapter for Wii U.
//
// Built for SDL 2.0.12 with "./configure --enable-hidapi".

int main()
{
    // Not necessary to correctly detect the gamecube adapter.
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_GAMECUBE, "1");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) << '\n';

    struct shutdown_sdl { ~shutdown_sdl() { SDL_Quit(); } } on_shutdown;

    // A window is required to poll events. Without it, the event loop
    // does not work properly.
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window{
        SDL_CreateWindow("GameCube Adapter Test",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         320, // Window width
                         240, // Window height
                         SDL_WINDOW_OPENGL),
        SDL_DestroyWindow
    };

    if (!window) {
        std::cout << "Could not create window.\n";
        return 1;
    }

    SDL_Event e;
    SDL_GameController* c = nullptr;;
    bool done = false;

    while (!done) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_CONTROLLERAXISMOTION:
                    if (std::abs(e.caxis.value) > 10000) {
                        std::cout << "axis " << SDL_GameControllerGetStringForAxis(static_cast<SDL_GameControllerAxis>(e.caxis.axis)) << ": "
                                  << e.caxis.value << std::endl;
                    }
                    break;

                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP:
                    std::cout << "button " << SDL_GameControllerGetStringForButton(static_cast<SDL_GameControllerButton>(e.cbutton.button)) << ": "
                              << (e.cbutton.state ? "pressed" : "released") << std::endl;
                    break;

                case SDL_CONTROLLERDEVICEADDED:
                    std::cout << "opened controller\n";
                    if (!c)
                        c = SDL_GameControllerOpen(e.cdevice.which);
                    break;

                case SDL_CONTROLLERDEVICEREMOVED:
                    std::cout << "removed controller\n";
                    if (c && !SDL_GameControllerGetAttached(c)) {
                        SDL_GameControllerClose(c);
                        c = nullptr;
                    }
                    break;

                case SDL_KEYDOWN:
                case SDL_QUIT:
                    std::cout << "received quit message\n";
                    done = true;
                    if (c) {
                        SDL_GameControllerClose(c);
                        c = nullptr;
                        std::cout << "removed controller\n";
                    }
                    break;

                default:
                    break;
            }
        }
    }

    return 0;
}

