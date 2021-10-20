#include <string>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>


// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// takes in a reference (see c++ video) to a string that is the *path* of the shader,
// and a "GLenum" that represents the type (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER) of the shader.
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

int main() {
    // Initialise GLFW
    int success = glfwInit();
    if (!success) {
        std::cerr << "failed to initialise GLFW" << std::endl;
        abort();
    }

    // Create our window
    // First two parameters are width/height, third is the title.
    GLFWwindow *window = glfwCreateWindow(1920, 1080, "Simple Shooter", nullptr, nullptr);
    if (!window) {
        std::cerr << "Window won't open" << std::endl;
        abort();
    }

    // make context current (not too important for our purposes)
    glfwMakeContextCurrent(window);

    // load OpenGL - should error check this as well
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // load shader
    GLuint vert_shader, frag_shader;
    vert_shader = load_shader("res/shaders/basic_vert.glsl", GL_VERTEX_SHADER);
    frag_shader = load_shader("res/shaders/basic_frag.glsl", GL_FRAGMENT_SHADER);

    // create our shader program that will run on the GPU
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

//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
//            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//
//            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//
//            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//            0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//
//            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//            0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//
//            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
//            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

//    std::vector<float> verts = {
//            // positions          // colors           // texture coords
//            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
//            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
//            //-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
//            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
//    };

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

    // Setup vertex array object and vertex buffer object

    // A buffer is a chunk of data that is stored on the GPU. It contains the positions
    // of our vertices.

    // A vertex array object describes how the data in our buffer/vbo is organised.
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Binding the vao and vbo lets OpenGL know that these are the vao/vbo we want to use
    // (as we can have multiple)
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // this puts the data into our buffer.
    glBufferData(GL_ARRAY_BUFFER, (GLintptr) verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);

    // We put "0" as the input to this function because we only have one attribute per vertex.
    // That attribute is the vertex's position. It is the first attribute, hence 0 (think about array indexing)
    // In the future, additional attributes such as texture co-ordinates may be included as well.
    // e.g. adding one more attribute would require a call to "glEnableVertexAttrib(1)".
    glEnableVertexAttribArray(0);

    // First parameter specifies the attribute to configure, the second is the number of items
    // in this attribute. Third is the type. The fourth is to do with normalization, and isn't particularly important
    // at this stage. The fifth is known as the "stride" and is the offset between (the start of) each vertex attribute
    // of this type. The final value is set to null right now, but maybe "0" would be a better value (it is set to
    // this anyway).
    //
    // Essentially, this becomes relevant when we have multiple attributes, as ths is the offset for
    // where the first occurrence of this attribute is in our buffer. We will explore this more in future weeks.
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

    // just in case - remove later
    glUseProgram(program);
    // set our texture sampler uniform
    glUniform1i(glGetUniformLocation(program, "ourTexture"),0);

    // Pass our projection matrix to the shader - got some magic nums here; being lazy
//    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.f), 800.0f / 600.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &projection[0][0]);

    // Render loop
    while(!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        process_input(window);

        // clear the colour buffer and set the background to black (try changing the first 3 values!)
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

        // draw the triangles
        for (auto &wallPos : wallPositions)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, wallPos);
            //float angle = 20.0f * i;
            //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            //ourShader.setMat4("model", model);

            glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
            glUseProgram(program);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
//        glm::mat4 model = glm::mat4(1.0f);
//        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);
//        glUseProgram(program);
//        glBindVertexArray(vao);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//        glBindVertexArray(0);

        //std::cout << "hihihih\n";

        // get events 
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
