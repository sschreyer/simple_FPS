#ifndef SIMPLE_SHOOTER_LOCATIONS_H
#define SIMPLE_SHOOTER_LOCATIONS_H

namespace locations {
    void setup_room();
    void draw_starting_room(GLuint reg_shader, GLuint light_shader, glm::mat4 proj, glm::mat4 view, glm::vec3 cam_pos);
}

#endif //SIMPLE_SHOOTER_LOCATIONS_H
