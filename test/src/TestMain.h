#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <ViXeL/ViXeL.h>
#include <ViXeL/Window.h>
#include <ViXeL/logic/Timing.h>
#include <ViXeL/rendering/camera/Camera.h>
#include <ViXeL/data/Vertex.h>
#include <ViXeL/model/Mesh.h>
#include <ViXeL/rendering/GLSLShaderProgram.h>

class TestMain {
public:
    TestMain();
    ~TestMain() = default;

    static bool running;

    void init();
    int loop();

    void processInput();
    void render();

    //Setters / Getters
    void setMaxFps(int fps) {
		_maxFps = fps;
	}

private:
    ViXeL::Window _window;

    int _windowWidth, _windowHeight;

    ViXeL::FpsLimiter _fpsLimiter;

    //Listeners
    bool _keyPressed, _mouseMoved, _mouseWheelScrolled, _mouseButtonPressed;

    //Shaders
    ViXeL::GLSLProgram _testShaderProgram;

    //Cameras
    ViXeL::Camera<ViXeL::PerspectiveCamera3D> _testCamera3d;

    //Meshes
    ViXeL::Mesh<ViXeL::ColoredVertex3D> _testCube;

    //Tool Windows
    

    GLuint _vbo, _vao;

    float _fps;
    float _maxFps = 60;
	int _vsync;
	float _gameTime;
};