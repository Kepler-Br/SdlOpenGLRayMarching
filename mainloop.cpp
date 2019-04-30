#include "mainloop.h"
#include <GL/glew.h>
#include <chrono>
#include "inputmanager.h"
//#include <GL/gl.h>


void MainLoop::init()
{
    const uint32_t points = 4;
    const uint32_t floatsPerPoint = 3;

    const GLfloat square[points][floatsPerPoint] =
    {
        { -1.0,  1.0,  0.0 }, // Top left
        {  1.0,  1.0,  0.0 }, // Top right
        {  1.0, -1.0,  0.0 }, // Bottom right
        { -1.0, -1.0,  0.0 }, // Bottom left
    };

    const uint32_t countVBOs = 1;

    glGenBuffers(countVBOs, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    uint32_t sizeInBytes = (points * floatsPerPoint) * sizeof(GLfloat);
    glBufferData(GL_ARRAY_BUFFER,sizeInBytes, square, GL_STATIC_DRAW);

    // VAO

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // call glBindBuffer() and glBindVertexArray() first
    // ...so that OpenGL knows what buffers we want to work on
    const uint32_t positionAttributeIndex = 0;
    glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    // Enable our attribute within the current VAO
    glEnableVertexAttribArray(positionAttributeIndex);
    shader.Init();

    glBindVertexArray(0);



}

void MainLoop::processEvents()
{
    if(InputManager::isKeyDown(SDLK_SPACE))
    {
        const glm::vec3 cameraPosition = camera.getPosition();
        const glm::vec3 up = camera.getUp();
        const glm::vec3 newPosition = up*customTime.deltaTime+cameraPosition;
        camera.setPosition(newPosition);
    }
    if(InputManager::isKeyDown(SDLK_c))
    {
        const glm::vec3 cameraPosition = camera.getPosition();
        const glm::vec3 down = -camera.getUp();
        const glm::vec3 newPosition = down*customTime.deltaTime+cameraPosition;
        camera.setPosition(newPosition);
    }
    if(InputManager::isKeyDown(SDLK_w))
    {
        const glm::vec3 cameraPosition = camera.getPosition();
        const glm::vec3 forward = camera.getForward();
        const glm::vec3 newPosition = cameraPosition + forward*customTime.deltaTime;
        camera.setPosition(newPosition);
    }
    if(InputManager::isKeyDown(SDLK_s))
    {
        const glm::vec3 cameraPosition = camera.getPosition();
        const glm::vec3 backward = -camera.getForward();
        const glm::vec3 newPosition = cameraPosition + backward*customTime.deltaTime;
        camera.setPosition(newPosition);
    }
    if(InputManager::isKeyDown(SDLK_d))
    {
        const glm::vec3 cameraPosition = camera.getPosition();
        const glm::vec3 right = camera.getRight();
        const glm::vec3 newPosition = right*customTime.deltaTime + cameraPosition;
        camera.setPosition(newPosition);
    }
    if(InputManager::isKeyDown(SDLK_a))
    {
        const glm::vec3 cameraPosition = camera.getPosition();
        const glm::vec3 left = -camera.getRight();
        const glm::vec3 newPosition = left*customTime.deltaTime + cameraPosition;
        camera.setPosition(newPosition);
    }
    if(InputManager::isKeyDown(SDLK_ESCAPE))
    {
         isRunning = false;
    }
    if(InputManager::isKeyDown(SDL_BUTTON_LEFT))
    {
//        camera.rotate(glm::vec3(0.0f, -InputManager::getDeltaMouseCoord().y/10.0f, 0.0f), 0);
//        camera.rotate(glm::vec3(0.0f, 0.0f, InputManager::getDeltaMouseCoord().x/100.0f));
        camera.rotate(glm::vec3(InputManager::getDeltaMouseCoord().y/40.0f, 0.0f, 0.0f), 0);
    }
    if(InputManager::isKeyDown(SDL_BUTTON_RIGHT))
    {
//        camera.rotate(glm::vec3(0.0f, -InputManager::getDeltaMouseCoord().y/10.0f, 0.0f), 0);
//        camera.rotate(glm::vec3(0.0f, 0.0f, InputManager::getDeltaMouseCoord().x/100.0f));
        camera.rotate(glm::vec3(0.0f, InputManager::getDeltaMouseCoord().x/40.0f, 0.0f), 0);
    }
}

void MainLoop::update()
{

}

void MainLoop::preRender()
{
    InputManager::update();
    window.clear();
    glBindVertexArray(vao);

    shader.UseProgram();
    GLint iTimeLocation = glGetUniformLocation(shader.shaderProgram, "iTime");
    if (iTimeLocation != -1)
    {
        using namespace std::chrono;
        float iTime = duration_cast<duration<float>>(steady_clock::now().time_since_epoch()).count();
        glUniform1f(iTimeLocation, static_cast<float>(iTime));
    }

    GLint iResolutionLocation = glGetUniformLocation(shader.shaderProgram, "iResolution");
    if (iResolutionLocation != -1)
    {
        const glm::ivec2 resolution = window.getGeometry();
        glUniform2f(iResolutionLocation, resolution.x, resolution.y);
    }

    GLint cameraPositionLocation = glGetUniformLocation(shader.shaderProgram, "cameraPosition");
    if (cameraPositionLocation != -1)
    {
        const glm::vec3 pos = camera.getPosition();
        glUniform3fv(cameraPositionLocation, 1, &pos[0]);
    }

    GLint projectionViewLocation = glGetUniformLocation(shader.shaderProgram, "inversedProjectionView");
    if (projectionViewLocation != -1)
    {
        const glm::mat4 PV = camera.getInversedPV();
        glUniformMatrix4fv(projectionViewLocation, 1, GL_FALSE, &PV[0][0]);
    }
}

void MainLoop::render()
{
    // Invoke glDrawArrays telling that our data is a line loop and we want to draw 2-4 vertexes
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void MainLoop::postRender()
{
    window.swapBuffer();
    glBindVertexArray(0);
}


void MainLoop::delayFps()
{
    if(1000.0f/fpsCap > customTime.deltaTime*1000.0f)
    {
        Uint32 delay = Uint32(1000.0f/fpsCap-customTime.deltaTime*1000.0f);
        SDL_Delay(delay);
    }
}

MainLoop::MainLoop():
    window(800, 600),
    camera(window)
{
    init();
}

MainLoop::~MainLoop()
{
    shader.CleanUp();
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void MainLoop::run()
{
    isRunning = true;

    while(isRunning)
    {
//        std::cout << "kek\n";
        customTime.calcDeltaTime();

        processEvents();
        update();

        preRender();
        render();
        postRender();

        delayFps();
    }
}
