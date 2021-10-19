// I don't think all of these are (currently) needed
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <utils.hpp>

namespace utils {
    // TO-DO: Add APPLE #define
    // make and return a pointer to a GLFWwindow
    GLFWwindow *make_window(int width, int height, std::string title) {
        GLFWwindow *window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
        if (!window) {
            std::cerr << "Window won't open" << std::endl;
            abort();
        }
        return window;
    }

    // a simple function to load a shader
    GLuint load_shader(const std::string &path, GLenum type) {
        std::string shaderCode;
        std::ifstream shaderFile;
        // ensure ifstream objects can throw exceptions:
        shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            shaderFile.open(path);
            std::stringstream shaderStream;
            // read file's buffer contents into streams
            shaderStream << shaderFile.rdbuf();
            // close file handlers
            shaderFile.close();
            // convert stream into string
            shaderCode   = shaderStream.str();
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }


        const char* src = shaderCode.data();

        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        return shader;
    }
}

