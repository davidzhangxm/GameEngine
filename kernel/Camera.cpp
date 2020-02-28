//
//  Camera.cpp
//  GameEngine
//
//  Created by Xinming Zhang on 2/27/20.
//  Copyright © 2020 Xinming Zhang. All rights reserved.
//

#include <iostream>
#include "Camera.hpp"
#include <glm/gtx/string_cast.hpp>

using namespace glm;

vec3 Camera::position;
vec3 Camera::front;
vec3 Camera::right;
vec3 Camera::up;
vec3 Camera::worldUp;
mat4 Camera::projection;
int Camera::width;
int Camera::height;

float Camera::Yaw;
float Camera::Pitch;
// camera options
float Camera::MovementSpeed;
float Camera::MouseSensitivity;
float Camera::Zoom;

bool Camera::cameraMoving;

shared_ptr<Camera> Camera::camera;

Camera::Camera(int width_, int height_)
{
    width = width_;
    height = height_;
    front = glm::vec3(0, 0, -1);
    MovementSpeed = SPEED;
    MouseSensitivity = SENSITIVITY;
    Zoom = ZOOM;
    init(vec3(0,20,15), vec3(0,1,0), 45.0);
}
void Camera::init(glm::vec3 pos_, glm::vec3 up_, double fov_, float yaw, float pitch)
{
    position = pos_;
    up = up_;
    worldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    cameraMoving = false;
    updateCameraVectors();
}

float Camera::getFOV()
{
    return Zoom;
}

void Camera::update_size(int width_, int height_)
{
    width = width_;
    height = height_;
}

glm::mat4 Camera::get_view()
{
    return glm::lookAt(position, position+front, up);
}

glm::mat4 Camera::get_projection()
{
    return perspective(glm::radians((double)Zoom), (double)width / (double)height, 1.0, 1000.0);
}

glm::mat4 Camera::getViewProjectionMatrix()
{
    return get_projection() * get_view();
}

shared_ptr<Camera> Camera::getInstance()
{
    if(camera == nullptr)
    {
        camera = make_shared<Camera>(1280, 960);
    }
    return camera;
}

void Camera::ProcessKeyBoard(Camera_Movement direction, float deltaTime)
{
    cameraMoving = true;
    float velocity = MovementSpeed * deltaTime;
    if(direction == FORWARD)
        position += front * velocity;
    if(direction == BACKWARD)
        position -= front * velocity;
    if(direction == LEFT)
        position -= right * velocity;
    if(direction == RIGHT)
        position += right * velocity;
//    printf("position is (%f, %f, %f)\n", position.x, position.y, position.z);
    cameraMoving = false;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;
    
    Yaw += xoffset;
    Pitch += yoffset;
    
    if(constrainPitch)
    {
        if(Pitch > 89.0f)
            Pitch = 89.0f;
        if(Pitch < -89.0f)
            Pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    if(Zoom >= 1.0f && Zoom <= 45.0f)
        Zoom -= yoffset;
    if(Zoom <= 1.0f)
        Zoom = 1.0f;
    if(Zoom >= 45.0f)
        Zoom = 45.0f;
}


void Camera::updateCameraVectors()
{
    vec3 front_;
    front_.x = cos(radians(Yaw)) * cos(radians(Pitch));
    front_.y = sin(radians(Pitch));
    front_.z = sin(radians(Yaw)) * cos(radians(Pitch));
    front = normalize(front_);
    right = normalize(cross(front, worldUp));
    up = normalize(cross(right, front));
}

bool Camera::isCameraMoved()
{
    return cameraMoving;
}

glm::vec3 Camera::getPosition()
{
    return position;
}
