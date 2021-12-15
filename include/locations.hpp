#ifndef SIMPLE_SHOOTER_LOCATIONS_H
#define SIMPLE_SHOOTER_LOCATIONS_H

#include <primitives.hpp>
#include <camera.hpp>
#include <light.hpp>
#include <vector>

namespace locations {
    enum {
        EMPTY,
        STATIC_MESH
    } kind;

    struct node_t {
        primitives::mesh_t mesh;
        GLuint maps[2];
        glm::mat4 transform = glm::mat4(1);
        std::vector<node_t> children{};
    };

    struct scene_t {
        camera::camera_t cam;

        std::vector<point_light_t> point_lights;

        node_t root;
    };

    scene_t setup_room();
}
#endif //SIMPLE_SHOOTER_LOCATIONS_H
