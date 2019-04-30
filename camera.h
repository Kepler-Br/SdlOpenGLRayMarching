#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

#include "window.h"

class Camera
{
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 projectionView;
    glm::mat4 inversedProjectionView;
    glm::ivec2 screenGeometry;

    glm::vec3 up;
    glm::vec3 forward;
    glm::vec3 position;
    glm::vec3 rotation;
    Window &window;

    glm::quat camera_quat;

    void setPerspective();

public:
    Camera(Window &window);
    glm::vec3 getPosition();
    glm::vec3 screenToWorld(glm::ivec2 screenPosition);
    void setPosition(const glm::vec3 &position);
    glm::mat4 getPV();
    glm::mat4 getInversedPV();

    glm::vec3 getForward();
    glm::vec3 getUp();
    glm::vec3 getRight();

    void updateView();

    void rotate(const glm::vec3 &axis, const float degree);
    glm::mat4 getInversedProjectionView() const;
    void setInversedProjectionView(const glm::mat4 &value);
};

#endif // CAMERA_H
