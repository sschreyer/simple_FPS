#ifndef SIMPLE_SHOOTER_MODEL_H
#define SIMPLE_SHOOTER_MODEL_H

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <primitives.hpp>

namespace model {


    struct material_t {
        // TODO: add more fields? colour?
        GLuint tex;
        GLuint specular;
        float shininess;
    };

    struct model_t {
        primitives::mesh_t mesh;
        struct material_t mat;
    };
}

#endif //SIMPLE_SHOOTER_MODEL_H
