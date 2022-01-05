#ifndef SIMPLE_SHOOTER_RENDERER_HPP
#define SIMPLE_SHOOTER_RENDERER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <locations.hpp>

#include <stack>

struct renderer_t {
    glm::mat4 projection;

    GLuint program;
    GLuint lightcube_program;
};

renderer_t make_renderer(const glm::mat4 &projection);
void render(const renderer_t &renderer, const locations::scene_t &scene);

#endif //SIMPLE_SHOOTER_RENDERER_HPP
