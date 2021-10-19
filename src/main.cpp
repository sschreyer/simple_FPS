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

// TO-DO: Move camera logic into a different file
// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

// TO-DO: delta helper function
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// handle all keyboard inputs
void process_input(GLFWwindow *window);

int main() {
    // TODO: Make util function
    // Initialise GLFW
    int success = glfwInit();
    if (!success) {
        std::cerr << "failed to initialise GLFW" << std::endl;
        abort();
    }

    GLFWwindow *window = utils::make_window(1920, 1080, "Simple Shooter");

    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);

    // load shader
    GLuint vert_shader, frag_shader;
    vert_shader = utils::load_shader("res/shaders/shader.vert", GL_VERTEX_SHADER);
    frag_shader = utils::load_shader("res/shaders/shader.frag", GL_FRAGMENT_SHADER);

    // create and link our shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);

    glUseProgram(program);


    // make basic rect
    primitives::mesh_t rect = primitives::make_rect();


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




    // TO-DO: texture class and/or file (probably file)
    // let's load + create our texture!
    GLuint cobble_texture;
    glGenTextures(1, &cobble_texture);
    glBindTexture(GL_TEXTURE_2D, cobble_texture);
    // set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture, generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("res/assets/Mossy_Cobblestone.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    // set our texture sampler uniform
    glUniform1i(glGetUniformLocation(program, "ourTexture"),0);

    // Pass our projection matrix to the shader - remove magic nums soon
    glm::mat4 projection = glm::perspective(glm::radians(45.f), 800.0f / 600.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &projection[0][0]);

    // Render loop
    while(!glfwWindowShouldClose(window)) {
        // calculate deltas
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_input(window);

        // clear colour buffer, set background.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera/view transformation
        glm::mat4 view = glm::mat4(1.0f); // initialize matrix to identity first
        float radius = 5.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        float camY = sin(glfwGetTime()) * radius;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, &view[0][0]);

        // bind our textures!
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cobble_texture);

        // draw the walls
        for (auto &wallPos : wallPositions)
        {
            // calc model matrix for every wall
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, wallPos);

            glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
            glUseProgram(program);
            glBindVertexArray(rect.vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // get events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}

// this is actually okay
void process_input(GLFWwindow *window) {

    // do stuff
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // handle camera!
    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *cameraSpeed;
}
