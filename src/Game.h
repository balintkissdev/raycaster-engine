#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Camera.h"
#include "IRenderer.h"
#include "Map.h"
#include "RayCaster.h"

class Game
{
public:
    Game();
    ~Game();

    bool init();
    int run();

#ifdef __EMSCRIPTEN__
    void runEmscriptenIteration();
#endif

private:
    // Input event collection for decoupling event polling from logic update processing
    struct EventState
    {
        bool moveForward = false;
        bool moveBackward = false;
        bool turnLeft = false;
        bool turnRight = false;
        bool strafeLeft = false;
        bool strafeRight = false;
        bool running = false;
    };

    static constexpr uint16_t WINDOW_WIDTH = 1024;
    static constexpr uint16_t WINDOW_HEIGHT = 768;

    static constexpr float BASE_MOVEMENT_SPEED = 0.035f;
    static constexpr float RUN_MOVEMENT_SPEED = BASE_MOVEMENT_SPEED + 0.04f;
    static constexpr float CURSOR_TURN_SPEED = 0.03f;

    static constexpr Vector2<float> INITIAL_POSITION = {4.5f, 4.5f};
    static constexpr Vector2<float> INITIAL_DIRECTION = {1.f, 0.f};
    static constexpr float INITIAL_FIELD_OF_VIEW = 60.f;

    Camera camera_;
    RayCaster raycaster_;
    Map map_;
    std::unique_ptr<IRenderer> renderer_;
    bool running_{false};
    EventState eventState_;

    void event();
    void update();
    void render();
};
