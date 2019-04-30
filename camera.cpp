#include "camera.h"
#include <glm/gtx/quaternion.hpp>

glm::mat4 Camera::getInversedProjectionView() const
{
    return inversedProjectionView;
}

void Camera::setInversedProjectionView(const glm::mat4 &value)
{
    inversedProjectionView = value;
}

void Camera::setPerspective()
{
    const float fov = 3.14f/6.0f;
    const glm::ivec2 windowGeometry = window.getGeometry();
    const float aspect = float(windowGeometry.x)/windowGeometry.y;
    const float near = 20.0f;
    const float far = 30.0f;
    projection = glm::perspective(fov, aspect, near, far);
//    const float left = -windowGeometry.x/20.0f;
//    const float right = windowGeometry.x/20.0f;
//    const float bottom = -windowGeometry.y/20.0f;
//    const float top = windowGeometry.y/20.0f;
//    const float left = -1;
//    const float right = 1;
//    const float bottom = -1;
//    const float top = 1;
//    projection = glm::ortho(left, right, bottom, top, near, far);
}

void Camera::setPosition(const glm::vec3& position)
{
    this->position = position;
    updateView();
}

glm::mat4 Camera::getPV()
{
    return projectionView;
}

glm::mat4 Camera::getInversedPV()
{
    return inversedProjectionView;
}

glm::vec3 Camera::getForward()
{
    return forward;
}

glm::vec3 Camera::getUp()
{
//    return up;
    return -glm::vec3(view[0][1], view[1][1], view[2][1]);
}

glm::vec3 Camera::getRight()
{
//    return glm::cross(getForward(), getUp());
    return glm::vec3(view[0][0], view[1][0], view[2][0]);
}

void Camera::updateView()
{
//    view = glm::lookAt(
//                position,                    // Camera location
//                forward, // Where camera is looking
//                up  // Camera head is facing up
//                );
//    view = glm::lookAt(
//                position, // Camera location
//                forward+position, // Where camera xis looking
//                up // Camera head is facing up
//                );


    //temporary frame quaternion from pitch,yaw,roll
      //here roll is not used
      glm::quat key_quat = glm::quat(glm::vec3(rotation.x, rotation.y, rotation.z));
      //reset values
//      key_pitch = key_yaw = key_roll = 0;
      rotation = glm::vec3(0.0f);

      //order matters,update camera_quat
      camera_quat = key_quat * camera_quat;
      camera_quat = glm::normalize(camera_quat);
      glm::mat4 rotate = glm::mat4_cast(camera_quat);

      glm::mat4 translate = glm::mat4(1.0f);
      translate = glm::translate(translate, -position);

      view = rotate * translate;



    projectionView = projection * view;
    inversedProjectionView = inverse(projectionView);
}

void Camera::rotate(const glm::vec3 &axis, const float degree)
{
//    std::cout << to_string(position) << std::endl;
//        forward = glm::rotate(forward, degree, axis);
//        up = glm::rotate(up, degree, axis);
//    glm::quat rot = glm::quat(axis);
//    rot = glm::normalize(rot);
//    forward = rot*forward;

//    forward = glm::vec4(forward, 1.0f)*glm::toMat4(rot);
//    up = glm::vec4(up, 1.0f)*glm::toMat4(rot);
//    forward = glm::toMat4(rot)*glm::vec4(forward, 1.0f);
//    up = glm::toMat4(rot)*glm::vec4(up, 1.0f);

//    glm::rotate(forward, rotation);
//    setPosition(position);

    std::cout << to_string(position) << std::endl;
    std::cout << to_string(camera_quat) << std::endl;
    std::cout << "to_string(rotation)" << std::endl;

    rotation = axis;

    updateView();
}

Camera::Camera(Window &window):
    window(window)
{
    up = glm::vec3(0.0f, 0.0f, 1.0f);
    forward = glm::vec3(0.0f, 0.0f, 1.0f);
    position = glm::vec3(0.0f, 1.0f, 0.0f);
    camera_quat = glm::quat(-0.998531, 0.000000, 0.000000, -0.054177);
    position = glm::vec3(0.000000, 1.000000, -5.340001);
    setPerspective();
//    setPosition(glm::vec3(0.0f, 1.2f, 0.0f));
    updateView();
}

glm::vec3 Camera::getPosition()
{
    return position;
}

glm::vec3 Camera::screenToWorld(glm::ivec2 screenPosition)
{
    // NORMALISED DEVICE SPACE
    double x = 2.0 * screenPosition.x / screenGeometry.x - 1;
    double y = 2.0 * screenPosition.y / screenGeometry.y - 1;

    // HOMOGENEOUS SPACE
    glm::vec4 screenPos = glm::vec4(x, -y, -1.0f, 1.0f);

    // Projection/Eye Space
//    glm::mat4 projectView = projection * view;
    glm::mat4 viewProjectionInverse = inverse(projectionView);

    glm::vec4 worldPos = viewProjectionInverse * screenPos;
    return glm::vec3(worldPos);
}
