#ifndef SIMPLE_SHOOTER_UTILS_H
#define SIMPLE_SHOOTER_UTILS_H

namespace utils {
    GLFWwindow *make_window(int width, int height, std::string title);
    GLuint load_shader(const std::string &path, GLenum type);
}

#endif //SIMPLE_SHOOTER_UTILS_H

