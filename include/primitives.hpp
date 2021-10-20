#ifndef SIMPLE_SHOOTER_PRIMITIVES_H
#define SIMPLE_SHOOTER_PRIMITIVES_H

#include <glad/glad.h>

namespace primitives {

    struct mesh_t {
        GLuint vao;
        GLuint vbo;
        // TODO - add EBO
    };

    mesh_t make_rect();
    mesh_t make_rect(int n_sq_h, int n_sq_v);

    mesh_t make_light_cube();
};

#endif //SIMPLE_SHOOTER_PRIMITIVES_H
