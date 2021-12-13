#ifndef SIMPLE_SHOOTER_LOCATIONS_H
#define SIMPLE_SHOOTER_LOCATIONS_H

#include <primitives.hpp>

namespace locations {
    enum {
        EMPTY,
        STATIC_MESH
    } kind;

    struct node_t {
        primitives::mesh_t mesh;
        GLuint maps[2];
        glm::mat4 transform = glm::mat4(1);
        std::vector<node_t> children {};
    };

    node_t setup_room();
    void draw_starting_room(GLuint reg_shader, GLuint light_shader, glm::mat4 proj, glm::mat4 view, glm::vec3 cam_pos);
}

#endif //SIMPLE_SHOOTER_LOCATIONS_H
