#include <glm/ext.hpp>

#include <camera.hpp>

namespace camera {
    // camera movement speed in units per second
    const float CAMERA_SPEED = 10.0f;


    camera_t make_camera(glm::vec3 pos, glm::vec3 target) {

        // target - pos will get us a vector from pos to target
        // final vector is our up vector!
        return {pos, glm::normalize(target - pos), glm::vec3(0, 1, 0),0,0};
    }

    glm::mat4 get_view(const camera_t &camera) {
        // glm::lookAt requires a position, a "target" (what we're looking at -
        // "The direction is the current position + the direction vector we just defined. This ensures that however we move,
        // the camera keeps looking at the target direction"),
        // and an up vector!
        return glm::lookAt(camera.pos, camera.pos + camera.front, camera.up);
    }

    // this is very straightforward
    void update_camera(camera_t &cam, GLFWwindow *window, float dt) {
        glm::vec3 front = cam.front;
        // We want movement speed to be consistent, so must normalise this
        glm::vec3 right = glm::normalize(glm::cross(front, cam.up));
        float step = dt * CAMERA_SPEED;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cam.pos += front * step;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cam.pos -= front * step;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cam.pos += right * step;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cam.pos -= right * step;
        }
    }
    // explanation on https://learnopengl.com/Getting-started/Camera
    // essentially, take the mouse movement as rotations (yaw and pitch) around the y and x axes respectively
    // x and y are just the mouse's position.
    void on_mouse_move(GLFWwindow *win, double x, double y) {
        static float xprev = x;
        static float yprev = y;

        // somewhat arbitrary sensitivity value (think about video games)
        float xoffset = 0.1f * (x - xprev);
        float yoffset = 0.1f * (yprev - y); // reversed since y-coordinates range from bottom to top

        xprev = x;
        yprev = y;

        camera_t *c = (camera_t *) glfwGetWindowUserPointer(win);
        float yaw = c->yaw + xoffset;
        float pitch = c->pitch + yoffset;

        float yaw_r = glm::radians(yaw);
        float pitch_r = glm::radians(pitch);
        // x and z are influenced by both pitch and yaw,
        // whereas y is only influenced by yaw.

        // go to LearnOpenGL for this
        c->front = glm::normalize(glm::vec3{
                std::cos(yaw_r) * std::cos(pitch_r),
                std::sin(pitch_r),
                std::sin(yaw_r) * std::cos(pitch_r)
        });

        c->pitch = pitch;
        c->yaw = yaw;
    }
}