#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <utils.hpp>
#include <primitives.hpp>
#include <camera.hpp>

// TODO: delta helper function? Possibly overkill.
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main() {
    // Initialise GLFW, make window, load OpenGL functions.
    GLFWwindow *window = utils::initial_setup();
    glEnable(GL_DEPTH_TEST);

    // load shader
    GLuint vert_shader, frag_shader;
    vert_shader = utils::load_shader("res/shaders/shader.vert", GL_VERTEX_SHADER);
    frag_shader = utils::load_shader("res/shaders/shader.frag", GL_FRAGMENT_SHADER);

    // create and link our shader program
    GLuint program = utils::make_shader(vert_shader, frag_shader);
    glUseProgram(program);


    // make basic rect
    primitives::mesh_t rect = primitives::make_rect(5, 3);

    // this can stay in this file for now.
    std::vector<glm::mat4> wallTransforms = {
            // first hallway
            glm::translate(glm::mat4(1), glm::vec3(0, 0, 3)),
            glm::translate(glm::mat4(1), glm::vec3(0, 0, -3)),
            glm::translate(glm::mat4(1), glm::vec3(0, 1.5, 0)) * glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(5,5,5)),
            glm::translate(glm::mat4(1), glm::vec3(0, -1.5, 0)) * glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(5,5,5))
    };

    GLuint cobble_texture = utils::make_texture("res/assets/Mossy_Cobblestone.png");


    // set our texture sampler uniform
    glUniform1i(glGetUniformLocation(program, "ourTexture"),0);

    // Pass our projection matrix to the shader - remove magic nums soon
    glm::mat4 projection = glm::perspective(glm::radians(45.f), 1920.0f / 1080.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &projection[0][0]);

    // make camera
    camera::camera_t cam = camera::make_camera(glm::vec3(0,0,1.5f), glm::vec3(0,0,0));

    // make camera move with mouse
    glfwSetWindowUserPointer(window, &cam);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, camera::on_mouse_move);

    // Render loop
    while(!glfwWindowShouldClose(window)) {
        // calculate deltas
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera::update_camera(cam, window, deltaTime);

        // clear colour buffer, set background.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind our textures!
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cobble_texture);

        // draw the walls
        for (auto &wallTranslate : wallTransforms)
        {
            // calc model matrix for every wall
            glm::mat4 model = glm::mat4(1.0f);
            model = wallTranslate;

            glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, glm::value_ptr(projection * camera::get_view(cam) * model));
            glUseProgram(program);
            glBindVertexArray(rect.vao);
            glBindTexture(GL_TEXTURE_2D, cobble_texture);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        // get events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}
