#ifndef SIMPLE_SHOOTER_CAMERA_H
#define SIMPLE_SHOOTER_CAMERA_H


#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace camera {
    struct camera_t {
        glm::vec3 pos;
        glm::vec3 front;
        glm::vec3 up;

        double pitch;
        double yaw;
    };

    camera_t make_camera(glm::vec3 pos, glm::vec3 target);
    void update_camera(camera_t &cam, GLFWwindow *window, float dt);
    glm::mat4 get_view(const camera_t &camera);
    void on_mouse_move(GLFWwindow *win, double x, double y);
}

#endif //SIMPLE_SHOOTER_CAMERA_H
