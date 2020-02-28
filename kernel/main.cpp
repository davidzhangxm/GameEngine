//
//  main.cpp
//  GameEngine
//
//  Created by Xinming Zhang on 2/27/20.
//  Copyright © 2020 Xinming Zhang. All rights reserved.
//

// system library
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <memory>

// project library
#include "Window.hpp"

// third party library
#include <glog/logging.h>

int main(int argc, const char * argv[]) {
    
    int window_width = 1280;
    int window_height = 960;
    const char* window_title = "My Engine";
    // Create the GLFW window.
    std::unique_ptr<Window> render_window = std::make_unique<Window>(window_width, window_height, window_title);
    if(!render_window->createWindow())
    {
        LOG(ERROR) << "window intialization error!";
        return -1;
    }

    // Print OpenGL and GLSL versions.
    render_window->print_versions();
    // Setup callbacks.
    render_window->setup_callbacks();
    // opengl setup
    render_window->setup_opengl_settings();

    render_window->event_loop();
}
