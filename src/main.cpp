#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <utils.hpp>
#include <camera.hpp>
#include <locations.hpp>
#include <renderer.hpp>

#include <thread>
#include <chrono>

/**
 * Returns the difference in time between when this function was previously called and this call.
 * @return A float representing the difference between function calls in seconds.
 */
float time_delta();

/**
 * Returns the current time in seconds.
 * @return Returns the current time in seconds.
 */
float time_now();

int main() {
    // Initialise GLFW, make window, load OpenGL functions.
    GLFWwindow *window = utils::initial_setup();
    glEnable(GL_DEPTH_TEST);

    // Pass our projection matrix to the shader - remove magic nums soon - TODO: possibly move this code as the flow isn't great now
    glm::mat4 projection = glm::perspective(glm::radians(45.f), 1920.0f / 1080.0f, 0.1f, 100.0f);
    renderer_t renderer = make_renderer(projection);

    // make starting room
    locations::scene_t scene = locations::setup_room();

    // make camera move with mouse - TODO: change?
    glfwSetWindowUserPointer(window, &scene.cam);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, camera::on_mouse_move);

    // Render loop
    while(!glfwWindowShouldClose(window)) {
        // calculate delta time
        float dt = time_delta();
        camera::update_camera(scene.cam, window, dt);

        // clear colour buffer, set background.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw the room
        render(renderer, scene);

        // get events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);

        // basic attempt at frame limiting
        std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1000.f / 60));
    }

    glfwTerminate();
}

float time_delta() {
    static float then = time_now();
    float now = time_now();
    float dt = now - then;
    then = now;
    return dt;
}

float time_now() {
    return (float) glfwGetTime();
}
