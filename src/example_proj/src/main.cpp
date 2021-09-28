#include <string>
#include <vector>
#include <iostream>

// our maths lib
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Our loader - loads OpenGL functions so we don't have to do this manually
#include <glad/glad.h>
// Windowing library - allows us to easily make windows in an OS-independent way
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>


// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// a simple function to load a shader
GLuint load_shader(const std::string &path, GLenum type) {
    std::string source = chicken3421::read_file(path);
    const char* src = source.data();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}

// handle all keyboard inputs
void process_input(GLFWwindow *window);

// make and return a pointer to a GLFWwindow
GLFWwindow *make_window(int width, int height, std::string title) {
    GLFWwindow *window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Window won't open" << std::endl;
        abort();
    }
    return window;
}

int main() {
    // Initialise GLFW
    int success = glfwInit();
    if (!success) {
        std::cerr << "failed to initialise GLFW" << std::endl;
        abort();
    }

    GLFWwindow *window = make_window(1920, 1080, "Simple Shooter");

    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);

    // load shader
    GLuint vert_shader, frag_shader;
    vert_shader = load_shader("res/shaders/shader.vert", GL_VERTEX_SHADER);
    frag_shader = load_shader("res/shaders/shader.frag", GL_FRAGMENT_SHADER);

    // create and link our shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);

    glUseProgram(program);

    // setup vertex data - each row is a vertex. 
    // first 3 points in each row are the vertex (x,y,z) co-ordinates, 
    // second 3 are the *color*
    std::vector<float> verts = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    };

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

    // setup vao and vbo for walls
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLintptr) verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *) 0);

    // Let's setup the color for our VAO
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *) (sizeof(float) * 3));

    // setup texture co-ordinates for our VAO
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8 , (void *) (sizeof(float) * 6));

    // Unbinds the vao
    glBindVertexArray(0);

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

/
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
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // get events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}

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
