#ifndef SIMPLE_SHOOTER_PRIMITIVES_H
#define SIMPLE_SHOOTER_PRIMITIVES_H
namespace primitives {

    struct mesh_t {
        GLuint vao;
        GLuint vbo;
        // TODO - add EBO
    };

    mesh_t make_rect();
};

#endif //SIMPLE_SHOOTER_PRIMITIVES_H
