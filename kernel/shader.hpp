//
//  shader.hpp
//  GameEngine
//
//  Created by Xinming Zhang on 2/27/20.
//  Copyright © 2020 Xinming Zhang. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <glm/glm.hpp>

class Program{
public:
    enum ShaderType { vertex, fragment, geometry };
    GLuint id;
    Program(){}
    Program(const char* vertex_file_path, const char* frag_file_path);
    Program(const char* vertex_file_path, const char* frag_file_path, const char* geo_file_path);
    void use();
    void unuse();
    
    void setBool(const char* name, bool value) const;
    void setInt(const char* name, int value) const;
    void setFloat(const char* name, float value) const;
    void setVec3(const char* name, glm::vec3 value) const;
    void setMat4(const char* name, glm::mat4 value) const;
private:
    
    GLuint LoadSingleShader(const char * shaderFilePath, ShaderType type);
    GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
    GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path, const char * geometry_file_path);
    
};


#endif /* shader_hpp */
