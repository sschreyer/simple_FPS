// I don't think all of these are (currently) needed
#include <glm/glm.hpp>
#include <glad/glad.h>

#include <vector>

#include <primitives.hpp>

namespace primitives {
    // TODO: generalise (currently not using this function, but a general make_rect would be useful)
    mesh_t make_rect() {
        // TODO: use ebo
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


        return {vao, vbo};
    }

    mesh_t make_rect(int n_sq_h, int n_sq_v) {
        std::vector<float> verts = {
                // x, y, z, u, v, n (x, y, z)
                // TODO: update normals? I don't think this will work properly for lighting...
                // actually assuming the direction is correct, may need to just flip it depending on where
                // the rect is translated to...
                -1.f * n_sq_h, -1.f * n_sq_v, 0.0f, 0.0f * n_sq_h, 0.0f * n_sq_v, 0, 0, -1.f,
                1.f * n_sq_h, -1.f * n_sq_v, 0.0f, 1.0f * n_sq_h, 0.0f * n_sq_v, 0, 0, -1.f,
                1.f * n_sq_h,  1.f  * n_sq_v, 0.0f, 1.0f * n_sq_h, 1.0f * n_sq_v, 0, 0, -1.f,
                -1.f * n_sq_h,  1.f * n_sq_v, 0.0f, 0.0f * n_sq_h, 1.0f * n_sq_v, 0, 0, -1.f
        };

        std::vector<int> indices = {
                0, 1, 2,
                0, 3, 2
        };

        // setup vao, vbo, ebo for walls
        GLuint vao, vbo, ebo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, (GLintptr) verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLintptr) indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *) 0);

        // setup texture co-ordinates for our VAO
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8 , (void *) (sizeof(float) * 3));

        // setup normals
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *) (sizeof(float) * 5));

        // Unbinds the vao
        glBindVertexArray(0);


        // TODO: ebo should be here lmao
        return {vao, vbo};
    }

    mesh_t make_cube(int size) {
               std::vector<float> verts = {
                // positions          // normals           // texture coords
                -1.f * size, -1.f * size, -1.f * size,  0.0f,  0.0f, 1.0f,  0.0f * size,  0.0f * size,
                1.f * size, -1.f * size, -1.f * size,  0.0f,  0.0f, 1.0f,  1.0f * size,  0.0f * size,
                1.f * size,  1.f * size, -1.f * size,  0.0f,  0.0f, 1.0f,  1.0f * size,  1.0f * size,
                1.f * size,  1.f * size, -1.f * size,  0.0f,  0.0f, 1.0f,  1.0f * size,  1.0f * size,
                -1.f * size,  1.f * size, -1.f * size,  0.0f,  0.0f, 1.0f,  0.0f * size,  1.0f * size,
                -1.f * size, -1.f * size, -1.f * size,  0.0f,  0.0f, 1.0f,  0.0f * size,  0.0f * size,

                -1.f * size, -1.f * size,  1.f * size,  0.0f,  0.0f,  -1.0f,  0.0f * size,  0.0f * size,
                1.f * size, -1.f * size,  1.f * size,  0.0f,  0.0f,  -1.0f,  1.0f * size,  0.0f * size,
                1.f * size,  1.f * size,  1.f * size,  0.0f,  0.0f,  -1.0f,  1.0f * size,  1.0f * size,
                1.f * size,  1.f * size,  1.f * size,  0.0f,  0.0f,  -1.0f,  1.0f * size,  1.0f * size,
                -1.f * size,  1.f * size,  1.f * size,  0.0f,  0.0f,  -1.0f,  0.0f * size,  1.0f * size,
                -1.f * size, -1.f * size,  1.f * size,  0.0f,  0.0f,  -1.0f,  0.0f * size,  0.0f * size,

                -1.f * size,  1.f * size,  1.f * size, 1.0f,  0.0f,  0.0f,  1.0f * size,  0.0f * size,
                -1.f * size,  1.f * size, -1.f * size, 1.0f,  0.0f,  0.0f,  1.0f * size,  1.0f * size,
                -1.f * size, -1.f * size, -1.f * size, 1.0f,  0.0f,  0.0f,  0.0f * size,  1.0f * size,
                -1.f * size, -1.f * size, -1.f * size, 1.0f,  0.0f,  0.0f,  0.0f * size,  1.0f * size,
                -1.f * size, -1.f * size,  1.f * size, 1.0f,  0.0f,  0.0f,  0.0f * size,  0.0f * size,
                -1.f * size,  1.f * size,  1.f * size, 1.0f,  0.0f,  0.0f,  1.0f * size,  0.0f * size,

                1.f * size,  1.f * size,  1.f * size,  -1.0f,  0.0f,  0.0f,  1.0f * size,  0.0f * size,
                1.f * size,  1.f * size, -1.f * size,  -1.0f,  0.0f,  0.0f,  1.0f * size,  1.0f * size,
                1.f * size, -1.f * size, -1.f * size,  -1.0f,  0.0f,  0.0f,  0.0f * size,  1.0f * size,
                1.f * size, -1.f * size, -1.f * size,  -1.0f,  0.0f,  0.0f,  0.0f * size,  1.0f * size,
                1.f * size, -1.f * size,  1.f * size,  -1.0f,  0.0f,  0.0f,  0.0f * size,  0.0f * size,
                1.f * size,  1.f * size,  1.f * size,  -1.0f,  0.0f,  0.0f,  1.0f * size,  0.0f * size,

                -1.f * size, -1.f * size, -1.f * size,  0.0f, 1.0f,  0.0f,  0.0f * size,  1.0f * size,
                1.f * size, -1.f * size, -1.f * size,  0.0f, 1.0f,  0.0f,  1.0f * size,  1.0f * size,
                1.f * size, -1.f * size,  1.f * size,  0.0f, 1.0f,  0.0f,  1.0f * size,  0.0f * size,
                1.f * size, -1.f * size,  1.f * size,  0.0f, 1.0f,  0.0f,  1.0f * size,  0.0f * size,
                -1.f * size, -1.f * size,  1.f * size,  0.0f, 1.0f,  0.0f,  0.0f * size,  0.0f * size,
                -1.f * size, -1.f * size, -1.f * size,  0.0f, 1.0f,  0.0f,  0.0f * size,  1.0f * size,

                -1.f * size,  1.f * size, -1.f * size,  0.0f,  -1.0f,  0.0f,  0.0f * size,  1.0f * size,
                1.f * size,  1.f * size, -1.f * size,  0.0f,  -1.0f,  0.0f,  1.0f * size,  1.0f * size,
                1.f * size,  1.f * size,  1.f * size,  0.0f,  -1.0f,  0.0f,  1.0f * size,  0.0f * size,
                1.f * size,  1.f * size,  1.f * size,  0.0f,  -1.0f,  0.0f,  1.0f * size,  0.0f * size,
                -1.f * size,  1.f * size,  1.f * size,  0.0f,  -1.0f,  0.0f,  0.0f * size,  0.0f * size,
                -1.f * size,  1.f * size, -1.f * size,  0.0f,  -1.0f,  0.0f,  0.0f * size,  1.0f * size
        };

        // setup vao, vbo
        GLuint vao, vbo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, (GLintptr) verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *) 0);

        // setup normals for our vao
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *) (sizeof(float) * 3));

        // setup tcs
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *) (sizeof(float) * 6));

        // Unbinds the vao
        glBindVertexArray(0);

        return {vao, vbo};
    }

    mesh_t make_light_cube() {
        // tcs exist because I haven't taken them out...they're not needed
        std::vector<float> verts = {
                // positions          // normals           // texture coords
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
                0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };

        // setup vao, vbo
        GLuint vao, vbo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, (GLintptr) verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *) 0);

        // setup normals for our vao
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *) (sizeof(float) * 3));

        // Unbinds the vao
        glBindVertexArray(0);

        return {vao, vbo};

    }
}