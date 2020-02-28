//
//  Camera.hpp
//  GameEngine
//
//  Created by Xinming Zhang on 2/27/20.
//  Copyright © 2020 Xinming Zhang. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <stdio.h>
#include <memory>

using namespace std;

const float YAW  = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 100.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM   = 45.0f;

enum Camera_Movement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:
    Camera(int width_, int height_);
    ~Camera() = default;
    
    static void init(glm::vec3 pos_, glm::vec3 up_, double fov, float yaw = YAW, float pitch = PITCH);
    static void update_size(int width_, int height_);
    
    static glm::mat4 get_view();
    static glm::mat4 get_projection();
    void rotate(glm::vec3 axis, float angle);
    static glm::mat4 getViewProjectionMatrix();
    static void ProcessKeyBoard(Camera_Movement direction, float deltaTime);
    static void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    static void ProcessMouseScroll(float yoffset);
    static bool isCameraMoved();
    
    static shared_ptr<Camera> getInstance();
    static glm::vec3 getPosition();
    static float getFOV();
    
private:
    static int width;
    static int height;

    static glm::vec3 position;
    static glm::vec3 front;
    static glm::vec3 right;
    static glm::vec3 up;
    static glm::vec3 worldUp;
    
    static glm::mat4 projection;
    static bool cameraMoving;
    
    // euler angles
    static float Yaw;
    static float Pitch;
    // camera options
    static float MovementSpeed;
    static float MouseSensitivity;
    static float Zoom;
    
    static void updateCameraVectors();
    
    static shared_ptr<Camera> camera;
};

#endif /* Camera_hpp */
