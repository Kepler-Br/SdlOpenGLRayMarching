#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>

#include "customtime.h"
#include "window.h"
#include "camera.h"
#include "shader.h"

class MainLoop
{
private:
    bool isRunning = false;
    const float fpsCap = 20.0f;

    GLuint vbo;
    GLuint vao;

    CustomTime customTime;
    Window window;
    Camera camera;
    Shader shader;

    void init();
    void processEvents();
    void update();
    void preRender();
    void render();
    void postRender();
    void delayFps();
public:
    MainLoop();
    ~MainLoop();
    void run();

};

#endif // MAINLOOP_H
