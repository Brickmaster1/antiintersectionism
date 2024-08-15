#pragma once

#include <ViXeL/event/EventManager.h>
#include <ViXeL/event/Event.h>
#include <SDL2/SDL_events.h>

class WindowEvent final : public ViXeL::Event {
public:
    void* action(void* event) override;
};

inline void registerEvents() {
    ViXeL::registerEvent<WindowEvent>(std::to_string(SDL_QUIT));
}