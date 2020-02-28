//
//  Window.hpp
//  GameEngine
//
//  Created by Xinming Zhang on 2/27/20.
//  Copyright © 2020 Xinming Zhang. All rights reserved.
//

#ifndef _WINDOW_H_
#define _WINDOW_H_
// opengl header

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// system header
#include <iostream>
#include <vector>
#include <memory>

// project header
#include "Camera.hpp"
#include "shader.hpp"
#include "RenderEngine.hpp"

// library header
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <glm/gtx/string_cast.hpp>


using namespace std;
class Window
{
public:
    Window(int width, int height, const char* window_title);
    ~Window();
    
    int width;
    int height;
    int r_width;
    int r_height;
    bool createWindow();
    
    // gl setup
    void print_versions();
    void setup_callbacks();
    void setup_opengl_settings();
    
    void event_loop();
    
private:
    
    const char* window_title;
    const char* glsl_version;
    GLFWwindow* window;
    
    void cleanUp();
    // callback
    static void resizeCallback(GLFWwindow* window, int width, int height);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void error_callback(int error, const char* description);
    
    void displayCallback(GLFWwindow* window);
    void idleCallback(GLFWwindow* window);
    
    void init();
    shared_ptr<RenderEngine> render_engine;
};

#endif

