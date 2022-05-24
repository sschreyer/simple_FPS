#ifndef SIMPLE_SHOOTER_UTILS_H
#define SIMPLE_SHOOTER_UTILS_H

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>

namespace utils {
    GLFWwindow *make_window(int width, int height, std::string title);
    GLFWwindow *initial_setup(int win_width, int win_height);
    GLuint load_shader(const std::string &path, GLenum type);
    GLuint make_shader(GLuint vert_shader, GLuint frag_shader);
    GLuint make_texture(const std::string &path);
    void process_input(GLFWwindow *window);
}

#endif //SIMPLE_SHOOTER_UTILS_H

