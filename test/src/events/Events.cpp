#include "Events.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#include "../TestMain.h"

void* WindowEvent::action(void* event) {
    switch (const auto windowEvent = static_cast<SDL_Event*>(event); windowEvent->type) {
    case SDL_QUIT:
        TestMain::running = false;
        break;
    }

    return nullptr;
}