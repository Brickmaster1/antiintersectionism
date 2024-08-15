#include "TestMain.h"

#include <ViXeL/event/EventManager.h>
#include <ViXeL/logic/InputManager.h>
#include "ViXeL/data/Vertex.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "events/Events.h"

bool TestMain::running = true;

TestMain::TestMain() :
    _windowWidth(1024),
    _windowHeight(768),
    _testCamera3d(_windowWidth, _windowHeight, {0.0f, 0.0f, 0.0f}, 1.0f, 45.0f, {-90.0f, 0.0f}, 0.1f),
    _fpsLimiter(_maxFps)
{
    registerEvents();
}

void TestMain::init() {
    ViXeL::init();
    SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);

    _window.create("test program", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);

    std::cout << glGetString(GL_VENDOR) << '\n';
    std::cout << glGetString(GL_RENDERER) << '\n';
    std::cout << glGetString(GL_VERSION) << '\n';

    //Set VSYNC
    SDL_GL_SetSwapInterval(_vsync);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _testShaderProgram.compileShaders("resources/Shaders/testShader.vert", "resources/Shaders/testShader.frag");
    _testShaderProgram.addAttribute("vertexPosition");
    _testShaderProgram.addAttribute("vertexColor");
    _testShaderProgram.linkShaders();

    constexpr float testCubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    _testCube.setOrigin({0.0f, 0.0f, 0.0f});

    for(int i = 0; i < 36; i++) {
        glm::vec3 vertex = glm::vec3(testCubeVertices[i*5], testCubeVertices[i*5 + 1], testCubeVertices[i*5 + 2]);
        glm::vec2 uv = glm::vec2(testCubeVertices[i*5 + 3], testCubeVertices[i*5 + 4]);
        _testCube.addVertex(ViXeL::ColoredVertex3D({vertex.x, vertex.y, vertex.z}, {255, 255, 255, 255}));
    }

    _testCube.create();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    ImGui_ImplSDL2_InitForOpenGL(_window, );
    ImGui_ImplOpenGL3_Init();

    _window.grabMouse();

    ViXeL::EventManager::getInstance().listenForEvent(std::to_string(SDL_KEYDOWN), &_keyPressed);
    ViXeL::EventManager::getInstance().listenForEvent(std::to_string(SDL_MOUSEMOTION), &_mouseMoved);
    ViXeL::EventManager::getInstance().listenForEvent(std::to_string(SDL_MOUSEWHEEL), &_mouseWheelScrolled);
    ViXeL::EventManager::getInstance().listenForEvent(std::to_string(SDL_MOUSEBUTTONDOWN), &_mouseButtonPressed);
}

int TestMain::loop() { while(running) {
    _fpsLimiter.begin();
    
    processInput();

    _testCamera3d.update();

    render();

    if(_window.isMouseGrabbed()) {
        SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
        _window.setMousePosition(_windowWidth / 2, _windowHeight / 2);
        SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
    }
    
    _fps = _fpsLimiter.endFrame();
} return 0; }


void TestMain::processInput() {
    ViXeL::EventManager::getInstance().parse();

    constexpr float CAMERA_PAN_SPEED = 0.01f;
    constexpr float CAMERA_ROTATE_SPEED = 0.05f;
    constexpr float CAMERA_ZOOM_SPEED = 0.45f;

    if (_keyPressed) {
        if (ViXeL::InputManager::getKeyState(SDLK_w) || ViXeL::InputManager::getKeyState(SDLK_a) || ViXeL::InputManager::getKeyState(SDLK_s) || ViXeL::InputManager::getKeyState(SDLK_d)) {
            glm::vec3 cameraPosModifier = glm::vec3(0.0f, 0.0f, 0.0f);
            if (ViXeL::InputManager::getKeyState(SDLK_w)) {
                cameraPosModifier = cameraPosModifier + glm::normalize(glm::vec3(_testCamera3d.getCameraFacing().x, 0.0f, _testCamera3d.getCameraFacing().z));
            }

            if (ViXeL::InputManager::getKeyState(SDLK_a)) {
                cameraPosModifier = cameraPosModifier - _testCamera3d.getCameraRight();
            }

            if (ViXeL::InputManager::getKeyState(SDLK_s)) {
                cameraPosModifier = cameraPosModifier - glm::normalize(glm::vec3(_testCamera3d.getCameraFacing().x, 0.0f, _testCamera3d.getCameraFacing().z));
            }

            if (ViXeL::InputManager::getKeyState(SDLK_d)) {
                cameraPosModifier = cameraPosModifier + _testCamera3d.getCameraRight();
            }

            if (cameraPosModifier != glm::vec3(0.0f, 0.0f, 0.0f)) {
                _testCamera3d.setPos(_testCamera3d.getPos() + glm::normalize(cameraPosModifier) * CAMERA_PAN_SPEED);
            }
        }

        if (ViXeL::InputManager::getKeyState(SDLK_e)) { _testCamera3d.setPos(_testCamera3d.getPos() + glm::vec3(0.0f, CAMERA_PAN_SPEED, 0.0f)); }

        if (ViXeL::InputManager::getKeyState(SDLK_q)) { _testCamera3d.setPos(_testCamera3d.getPos() + glm::vec3(0.0f, -CAMERA_PAN_SPEED, 0.0f)); }


        if (ViXeL::InputManager::getKeyState(SDLK_LALT)) {
            _window.ungrabMouse();
        } else if (!_window.isMouseGrabbed()) {
            _window.grabMouse();
        }
    }
    
    if (!_window.isFocused()) { return; }

    if (_mouseMoved && !ViXeL::InputManager::getKeyState(SDLK_LALT)) {
        if (_testCamera3d.getRotationDegrees().x != -89.99f && _testCamera3d.getRotationDegrees().x != 89.99f) {
            float mouseVertical = _testCamera3d.getRotationDegrees().y - (ViXeL::InputManager::getMousePos().y * CAMERA_ROTATE_SPEED);
            if (mouseVertical > -89.99f && mouseVertical < 89.99f) {
                _testCamera3d.setRotationDegrees(
                    _testCamera3d.getRotationDegrees().x,
                    mouseVertical
                );
            } else if (mouseVertical > 89.99f) {
                _testCamera3d.setRotationDegrees(
                    _testCamera3d.getRotationDegrees().x,
                    89.99f
                );
            } else {
                _testCamera3d.setRotationDegrees(
                    _testCamera3d.getRotationDegrees().x,
                    -89.99f
                );
            }
        }
        
        _testCamera3d.setRotationDegrees(_testCamera3d.getRotationDegrees() + glm::vec2(
            ViXeL::InputManager::getMousePos().x * CAMERA_ROTATE_SPEED,
            0.0f
        ));

        _mouseMoved = false;
    }

    if (_mouseWheelScrolled) {
        if (_testCamera3d.getFovDegrees() < 89.95f) {
            float zoomAmount = _testCamera3d.getFovDegrees() - (ViXeL::InputManager::getScrollDirection().y * CAMERA_ZOOM_SPEED);
            if(zoomAmount > 15.0f && zoomAmount < 89.95f) {
                _testCamera3d.setFovDegrees(zoomAmount);
            } else if (zoomAmount < 15.0f) {
                _testCamera3d.setFovDegrees(15.0f);
            }
        } else if (_testCamera3d.getFovDegrees() > 15.0f) {
            float zoomAmount = _testCamera3d.getFovDegrees() - (ViXeL::InputManager::getScrollDirection().y * CAMERA_ZOOM_SPEED);
            if(zoomAmount > 15.0f && zoomAmount < 89.95f) {
                _testCamera3d.setFovDegrees(zoomAmount);
            } else if (zoomAmount > 89.95f) {
                _testCamera3d.setFovDegrees(89.95f);
            }
        }
        _mouseWheelScrolled = false;
    }

    if (_mouseButtonPressed) {

    }
}

void TestMain::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    _testShaderProgram.bind();
    // WIREFRAME START
    _testCube.render();
    // WIREFRAME END
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    _testCube.setOrigin({0.0f, 4.0f, 0.0f});

    _testShaderProgram.unbind();
    _window.swapBuffer();
}