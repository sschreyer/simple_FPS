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
#include <renderer.hpp>

#include <iostream>
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


//    GLuint cobble_texture = utils::make_texture("res/assets/Mossy_Cobblestone.png");
    model::material_t cobble_materials;
//    cobble_materials.tex = cobble_texture;

    GLuint cobble_spec = utils::make_texture("res/assets/Mossy_Cobblestone_SPEC.png");
//    cobble_materials.specular = cobble_spec;

    // TODO: alter values??
    //cobble_materials.shininess = 64;


    // Pass our projection matrix to the shader - remove magic nums soon - TODO: possibly move this code as the flow isn't great now
    glm::mat4 projection = glm::perspective(glm::radians(45.f), 1920.0f / 1080.0f, 0.1f, 100.0f);
    renderer_t renderer = make_renderer(projection);

    // not sure if needed
//    glUniform1i(glGetUniformLocation(renderer.program, "material.diffuse"), 0);
//    glUniform1i(glGetUniformLocation(renderer.program, "material.specular"), 1);
    glUniform1f(glGetUniformLocation(renderer.program, "material.shininess"), 64);

//
//
//    // setup light shader
//    // load shader
//    GLuint vert_shader1, frag_shader1;
//    vert_shader1 = utils::load_shader("res/shaders/light_src_vert.glsl", GL_VERTEX_SHADER);
//    frag_shader1 = utils::load_shader("res/shaders/light_src_frag.glsl", GL_FRAGMENT_SHADER);
//
//    // create and link our shader program
//    GLuint light_shader = utils::make_shader(vert_shader1, frag_shader1);
//    //glUseProgram(light_shader);

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

        // bind our textures!
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, cobble_materials.tex);
//
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, cobble_materials.specular);

        // draw the room
        render(renderer, scene);

        // get events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);

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
