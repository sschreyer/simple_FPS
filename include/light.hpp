#ifndef SIMPLE_SHOOTER_LIGHT_HPP
#define SIMPLE_SHOOTER_LIGHT_HPP

#include <glm/glm.hpp>

struct directional_light_t {
    glm::vec3 ambient;
    glm::vec3 diffuse;
};

struct point_light_t {
    glm::vec3 pos;
    glm::vec3 color;
    float constant;
    float linear;
    float quadratic;
};

#endif //SIMPLE_SHOOTER_LIGHT_HPP

