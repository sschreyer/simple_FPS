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
#include <thread>
#include <chrono>

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

    GLuint cobble_spec = utils::make_texture("res/assets/Mossy_Cobblestone_SPEC.png");
    cobble_materials.specular = cobble_spec;

    // TODO: alter values??
    //cobble_materials.shininess = 64;


    // not sure if needed

    glUniform1i(glGetUniformLocation(reg_shader, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(reg_shader, "material.specular"), 1);
    glUniform1f(glGetUniformLocation(reg_shader, "material.shininess"), 64);

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
    camera::camera_t cam = camera::make_camera(glm::vec3(0.f,0.f,1.5f), glm::vec3(0,0,0));

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
        glBindTexture(GL_TEXTURE_2D, cobble_materials.tex);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cobble_materials.specular);

        // draw the room
        locations::setup_room();
        locations::draw_starting_room(reg_shader, light_shader, projection, camera::get_view(cam), cam.pos);

        // get events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);

        std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1000.f / 60));
    }

    glfwTerminate();
}
