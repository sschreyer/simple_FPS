#ifndef SIMPLE_SHOOTER_MODEL_H
#define SIMPLE_SHOOTER_MODEL_H

#include <glm/glm.hpp>

#include <glad/glad.h>

namespace model {
    struct material_t {
        // TODO: add more fields? - spec map for one thing. colour?
        GLuint tex;
        glm::vec3 specular;
        float shininess;
    };
}

#endif //SIMPLE_SHOOTER_MODEL_H
