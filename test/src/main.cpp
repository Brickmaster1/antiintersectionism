#define SDL_MAIN_HANDLED

#include "TestMain.h"

static bool running = true;

int main() {
    TestMain mainProgram = TestMain();
    mainProgram.init();
    return mainProgram.loop();
}