//
//  shader.cpp
//  GameEngine
//
//  Created by Xinming Zhang on 2/27/20.
//  Copyright © 2020 Xinming Zhang. All rights reserved.
//

#include "shader.hpp"

Program::Program(const char* vertex_file_path, const char* frag_file_path)
{
    id = LoadShaders(vertex_file_path, frag_file_path);
}
Program::Program(const char *vertex_file_path, const char *fragment_file_path, const char *geometry_file_path)
{
    id = LoadShaders(vertex_file_path, fragment_file_path, geometry_file_path);
}

GLuint Program::LoadSingleShader(const char * shaderFilePath, ShaderType type)
{
    // Create a shader id.
    GLuint shaderID = 0;
    if (type == vertex)
        shaderID = glCreateShader(GL_VERTEX_SHADER);
    else if (type == fragment)
        shaderID = glCreateShader(GL_FRAGMENT_SHADER);
    else if (type == geometry)
        shaderID = glCreateShader(GL_GEOMETRY_SHADER);

    // Try to read shader codes from the shader file.
    std::string shaderCode;
    std::ifstream shaderStream(shaderFilePath, std::ios::in);
    if (shaderStream.is_open())
    {
        std::string Line = "";
        while (getline(shaderStream, Line))
            shaderCode += "\n" + Line;
        shaderStream.close();
    }
    else
    {
        std::cerr << "Impossible to open " << shaderFilePath << ". "
            << "Check to make sure the file exists and you passed in the "
            << "right filepath!"
            << std::endl;
        return 0;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Shader.
    std::cerr << "Compiling shader: " << shaderFilePath << std::endl;
    char const * sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer, NULL);
    glCompileShader(shaderID);
    
    // Check Shader.
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> shaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(shaderID, InfoLogLength, NULL, shaderErrorMessage.data());
        std::string msg(shaderErrorMessage.begin(), shaderErrorMessage.end());
        std::cerr << msg << std::endl;
        return 0;
    }
    else
    {
        if (type == vertex)
            printf("Successfully compiled vertex shader!\n");
        else if (type == fragment)
            printf("Successfully compiled fragment shader!\n");
        else if(type == geometry)
            printf("Successfully compiled geometry shader!\n");
    }

    return shaderID;
}

GLuint Program::LoadShaders(const char * vertexFilePath, const char * fragmentFilePath)
{
    // Create the vertex shader and fragment shader.
    GLuint vertexShaderID = LoadSingleShader(vertexFilePath, vertex);
    GLuint fragmentShaderID = LoadSingleShader(fragmentFilePath, fragment);

    // Check both shaders.
    if (vertexShaderID == 0 || fragmentShaderID == 0) return 0;

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Link the program.
    printf("Linking program\n");
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program.
    glGetProgramiv(programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(programID, InfoLogLength, NULL, ProgramErrorMessage.data());
        std::string msg(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
        std::cerr << msg << std::endl;
        glDeleteProgram(programID);
        return 0;
    }
    else
    {
        printf("Successfully linked program!\n");
    }

    // Detach and delete the shaders as they are no longer needed.
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}
GLuint Program::LoadShaders(const char *vertex_file_path, const char *fragment_file_path, const char *geometry_file_path)
{
    // Create the vertex shader and fragment shader.
    GLuint vertexShaderID = LoadSingleShader(vertex_file_path, vertex);
    GLuint fragmentShaderID = LoadSingleShader(fragment_file_path, fragment);
    GLuint geometryShaderID = LoadSingleShader(geometry_file_path, geometry);

    // Check both shaders.
    if (vertexShaderID == 0 || fragmentShaderID == 0 || geometryShaderID == 0) return 0;

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Link the program.
    printf("Linking program\n");
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glAttachShader(programID, geometryShaderID);
    glLinkProgram(programID);

    // Check the program.
    glGetProgramiv(programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(programID, InfoLogLength, NULL, ProgramErrorMessage.data());
        std::string msg(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
        std::cerr << msg << std::endl;
        glDeleteProgram(programID);
        return 0;
    }
    else
    {
        printf("Successfully linked program!\n");
    }

    // Detach and delete the shaders as they are no longer needed.
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    glDetachShader(programID, geometryShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    glDeleteShader(geometryShaderID);

    return programID;
}
void Program::use()
{
    glUseProgram(id);
}
void Program::unuse()
{
    glUseProgram(0);
}

void Program::setBool(const char *name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name), (int)value);
}

void Program::setInt(const char *name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name), value);
}
void Program::setFloat(const char *name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name), value);
}
void Program::setVec3(const char *name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(id, name), 1, &value[0]);
}
void Program::setMat4(const char *name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, &value[0][0]);
}
