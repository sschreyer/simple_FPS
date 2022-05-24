// I don't think all of these are (currently) needed
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <utils.hpp>


namespace utils {
    // TODO: Add APPLE #define
    // make and return a pointer to a GLFWwindow
    GLFWwindow *make_window(int width, int height, std::string title) {
        GLFWwindow *window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
        if (!window) {
            std::cerr << "Window won't open" << std::endl;
            abort();
        }
        return window;
    }

    GLFWwindow *initial_setup(int win_width, int win_height) {
        int success = glfwInit();
        if (!success) {
            std::cerr << "failed to initialise GLFW" << std::endl;
            abort();
        }

        // TODO: make width/height dynamic
        GLFWwindow *window = utils::make_window(win_width, win_height, "Simple Shooter");

        glfwMakeContextCurrent(window);
        gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

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

    GLuint make_shader(GLuint vert_shader, GLuint frag_shader) {
        GLuint program = glCreateProgram();
        glAttachShader(program, vert_shader);
        glAttachShader(program, frag_shader);
        glLinkProgram(program);
        return program;
    }

    GLuint make_texture(const std::string &path) {
        // let's load + create our texture!
        GLuint tex;
        glGenTextures(1, &tex);

        // don't believe this line is needed but I think it's good practice - esp. with Macs being Macs
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        // set texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load image, create texture, generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(path.data(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        return tex;
    }

}

