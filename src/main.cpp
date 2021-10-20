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
#include <locations.hpp>
#include <model.hpp>

#include <iostream>

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
    vert_shader = utils::load_shader("res/shaders/basic_vert.glsl", GL_VERTEX_SHADER);
    frag_shader = utils::load_shader("res/shaders/basic_frag.glsl", GL_FRAGMENT_SHADER);

    // create and link our shader program
    GLuint reg_shader = utils::make_shader(vert_shader, frag_shader);
    glUseProgram(reg_shader);


    GLuint cobble_texture = utils::make_texture("res/assets/Mossy_Cobblestone.png");
    model::material_t cobble_materials;
    cobble_materials.tex = cobble_texture;
    // TODO: alter values??
    cobble_materials.shininess = 0.1;

    // set our texture sampler uniform
    glUniform1i(glGetUniformLocation(reg_shader, "ourTexture"),0);

    // Pass our projection matrix to the shader - remove magic nums soon - TODO: possibly move this code as the flow isn't great now
    glm::mat4 projection = glm::perspective(glm::radians(45.f), 1920.0f / 1080.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(reg_shader, "projection"), 1, GL_FALSE, &projection[0][0]);

    // setup light shader
    // load shader
    GLuint vert_shader1, frag_shader1;
    vert_shader = utils::load_shader("res/shaders/light_src_vert.glsl", GL_VERTEX_SHADER);
    frag_shader = utils::load_shader("res/shaders/light_src_frag.glsl", GL_FRAGMENT_SHADER);

    // create and link our shader program
    GLuint light_shader = utils::make_shader(vert_shader, frag_shader);
    //glUseProgram(light_shader);

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

        // draw the room
        locations::setup_room();
        locations::draw_starting_room(reg_shader, light_shader, projection, camera::get_view(cam));

        // get events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}
