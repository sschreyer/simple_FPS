#include <string>
#include <vector>
#include <iostream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

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
    primitives::mesh_t rect = primitives::make_rect();

    // this can stay in this file for now.
    std::vector<glm::vec3> wallPositions = {
            // first hallway

            // 1st side
            // 1st row
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 0.0f, 0.0f),
            glm::vec3(3.0f, 0.0f, 0.0f),
            glm::vec3(4.0f, 0.0f, 0.0f),
            // 2nd row
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 0.0f),
            glm::vec3(2.0f, 1.0f, 0.0f),
            glm::vec3(3.0f, 1.0f, 0.0f),
            glm::vec3(4.0f, 1.0f, 0.0f),

            // 2nd side
            // 1st rwo
            glm::vec3( 0.0f,  0.0f, 4.0f),
            glm::vec3(1.0f, 0.0f, 4.0f),
            glm::vec3(2.0f, 0.0f, 4.0f),
            glm::vec3(3.0f, 0.0f, 4.0f),
            glm::vec3(4.0f, 0.0f, 4.0f),

            // 2nd row
            glm::vec3(0.0f, 1.0f, 4.0f),
            glm::vec3(1.0f, 1.0f, 4.0f),
            glm::vec3(2.0f, 1.0f, 4.0f),
            glm::vec3(3.0f, 1.0f, 4.0f),
            glm::vec3(4.0f, 1.0f, 4.0f)
    };

    GLuint cobble_texture = utils::make_texture("res/assets/Mossy_Cobblestone.png");


    // set our texture sampler uniform
    glUniform1i(glGetUniformLocation(program, "ourTexture"),0);

    // Pass our projection matrix to the shader - remove magic nums soon
    glm::mat4 projection = glm::perspective(glm::radians(45.f), 1920.0f / 1080.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &projection[0][0]);

    // make camera
    camera::camera_t cam = camera::make_camera(glm::vec3(0,0,5), glm::vec3(0,0,0));

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
        for (auto &wallPos : wallPositions)
        {
            // calc model matrix for every wall
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, wallPos);

            glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, glm::value_ptr(projection * camera::get_view(cam) * model));
            glUseProgram(program);
            glBindVertexArray(rect.vao);
            glBindTexture(GL_TEXTURE_2D, cobble_texture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // get events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}
