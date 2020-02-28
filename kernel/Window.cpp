//
//  Window.cpp
//  GameEngine
//
//  Created by Xinming Zhang on 2/27/20.
//  Copyright © 2020 Xinming Zhang. All rights reserved.
//

#include "Window.hpp"
#include "Camera.hpp"
#include <glog/logging.h>

Window::Window(int width, int height, const char* window_title)
{
    this->width = width;
    this->height = height;
    this->window_title = window_title;
    this->glsl_version = "#version 330";
    r_width = width;
    r_height = height;
}

bool Window::createWindow()
{
    // Initialize GLFW.
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.

    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    this->window = glfwCreateWindow(width, height, window_title, nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    if(!window)
    {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

    // Initialize GLEW.
    if (glewInit())
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }
#endif
    // Set swap interval to 1.
    glfwSwapInterval(0);

    // Call the resize callback to make sure things get drawn immediately.
    resizeCallback(window, width, height);

    return true;

}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(window);
    glfwTerminate();

}

void Window::print_versions()
{
    // Get info of GPU and supported OpenGL version.
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
        << std::endl;

    //If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
    std::cout << "Supported GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
    int r_w = width;
    int r_h = height;
#ifdef __APPLE__
    // In case your Mac has a retina display.
    // the window size will be double here
    // e.g. 640 x 480 => 1280 x 960
    // TODO
    glfwGetFramebufferSize(window, &width, &height);
#endif
    
    Window* _this = static_cast<Window*>(glfwGetWindowUserPointer(window));
    _this->height = height;
    _this->width = width;
    _this->r_width = r_w;
    _this->r_height = r_h;
    // Set the viewport size.
    glViewport(0, 0, width, height);
    // set camera projection here
    Camera::getInstance()->update_size(width, height);
}

void Window::error_callback(int error, const char* description)
{
    // Print error.
    std::cerr << description << std::endl;
}

float deltaTime;
float lastFrame;



void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    /*
     * TODO: Modify below to add your key callbacks.
     */
    // Check for a key press.
    auto _this = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if(action == GLFW_PRESS)
    {
        switch(key)
        {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            default:
                break;
        }

    }
}

void Window::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    Camera::getInstance()->ProcessMouseScroll(yoffset);
}

bool firstMouse;
float lastX;
float lastY;

void Window::cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    
    lastX = xpos;
    lastY = ypos;
    Camera::getInstance()->ProcessMouseMovement(xoffset, yoffset);
}

void Window::setup_callbacks()
{
    // Set the error callback.
    glfwSetErrorCallback(error_callback);
    // Set the key callback.
    glfwSetKeyCallback(window, keyCallback);
    // Set the window resize callback.
    glfwSetWindowSizeCallback(window, resizeCallback);
    // set scroll call back
    glfwSetScrollCallback(window, scroll_callback);
//    // set mouse call back
    glfwSetCursorPosCallback(window, Window::cursor_position_callback);
//    glfwSetMouseButtonCallback(window, Window::mouse_button_callback);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Camera::getInstance()->ProcessKeyBoard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Camera::getInstance()->ProcessKeyBoard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Camera::getInstance()->ProcessKeyBoard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Camera::getInstance()->ProcessKeyBoard(RIGHT, deltaTime);
}

void Window::event_loop()
{
    init();
    deltaTime = 0.0f;
    lastFrame = 0.0f;
    firstMouse = true;
    lastX = width / 2.0f;
    lastY = height / 2.0f;
    // imgui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    ImGui::StyleColorsDark();

    
    
    while(!glfwWindowShouldClose(window))
    {
        displayCallback(window);
        idleCallback(window);
    }
}

void Window::setup_opengl_settings()
{
    // Enable depth buffering.
    glEnable(GL_DEPTH_TEST);
    // Related to shaders and z value comparisons for the depth buffer.
    glDepthFunc(GL_LEQUAL);
    // Set polygon drawing mode to fill front and back of each polygon.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Set clear color to black.
    glClearColor(0.0, 0.0, 0.0, 0.0);
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
}

void Window::init()
{
    render_engine = make_shared<RenderEngine>();
    render_engine->init();
}
void Window::displayCallback(GLFWwindow *window)
{
    // per frame time logic
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    render_engine->render(deltaTime);
    render_engine->update();

    glfwPollEvents();
    
    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // frame rate
    ImGui::Begin("Performance Analysis");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    glfwSwapBuffers(window);
    
    processInput(window);
}

void Window::idleCallback(GLFWwindow *window)
{
    
}
