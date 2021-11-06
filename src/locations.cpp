#include <primitives.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glfw/glfw3.h>

#include <vector>

#include <locations.hpp>
#include <model.hpp>

namespace locations {

    primitives::mesh_t room;
    primitives::mesh_t light;
    std::vector<glm::vec3> light_positions;

    void setup_room() {
        room = primitives::make_rect_prism(40,3,5);
        light = primitives::make_light_cube();
        light_positions = { {0, 0, 2}, {10, 0, 2} };
    }

    void draw_starting_room(GLuint reg_shader, GLuint light_shader, glm::mat4 proj, glm::mat4 view, glm::vec3 cam_pos) {
        // draw room
        glUseProgram(reg_shader);

        // light uniforms
        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[0].ambient"), 0.15f, 0.15f, 0.15f);
        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[0].position"), light_positions[0].x, light_positions[0].y, light_positions[0].z);
        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[0].diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[0].specular"), 0.2f, 0.2f, 0.2f);
        // light attenutation
        glUniform1f(glGetUniformLocation(reg_shader, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(reg_shader, "pointLights[0].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(reg_shader, "pointLights[0].quadratic"), 0.20f);

        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[1].ambient"), 0.15f, 0.15f, 0.15f);
        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[1].position"), light_positions[1].x, light_positions[1].y, light_positions[1].z);
        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[1].diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[1].specular"), 0.2f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(reg_shader, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(reg_shader, "pointLights[1].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(reg_shader, "pointLights[1].quadratic"), 0.20f);


        glUniform3f(glGetUniformLocation(reg_shader, "camPos"), cam_pos.x, cam_pos.y, cam_pos.z);

        glm::mat4 model = glm::mat4(1); //glm::translate(glm::mat4(1), glm::vec3(0, 1, 15)); //glm::mat4(1.0f);

        glUniformMatrix4fv(glGetUniformLocation(reg_shader, "pv"), 1, GL_FALSE, glm::value_ptr(proj * view));
        glUniformMatrix4fv(glGetUniformLocation(reg_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(room.vao);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        // draw light cube
        for (auto light_pos : light_positions) {
            model = glm::translate(glm::mat4(1), light_pos);
            model *= glm::scale(glm::mat4(1), glm::vec3(0.3, 0.3, 0.3));

            glUseProgram(light_shader);
            glUniformMatrix4fv(glGetUniformLocation(light_shader, "mvp"), 1, GL_FALSE,
                               glm::value_ptr(proj * view * model));
            glBindVertexArray(light.vao);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    // TODO: fix or delete below (old) code...using a cube for the room for now (simpler)
//    primitives::mesh_t wall;
//    primitives::mesh_t roof;
//    primitives::mesh_t light;
//
//    std::vector<glm::mat4> wall_positions;
//    std::vector<glm::mat4> roof_positions;

    // TODO: probably pass in width and height for room? make it make a closed room? who knows...
//    void setup_room() {
//        wall = primitives::make_rect(20, 3);
//        roof = primitives::make_rect(20, 12);
//        light = primitives::make_light_cube();
//
//        wall_positions = {
//                glm::translate(glm::mat4(1), glm::vec3(0, 0, 12)),
//                glm::translate(glm::mat4(1), glm::vec3(0, 0, -12))
//        };
//
//        roof_positions = {
//                glm::translate(glm::mat4(1), glm::vec3(0, 3, 0)) * glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(1, 0, 0)),
//                glm::translate(glm::mat4(1), glm::vec3(0, -3, 0)) * glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(1, 0, 0))
//        };
//    }
//
//    void draw_starting_room(GLuint reg_shader, GLuint light_shader, glm::mat4 proj, glm::mat4 view) {
//        // draw walls
//        glUseProgram(reg_shader);
//        glUniform3f(glGetUniformLocation(reg_shader, "light.ambient"), 0.2f, 0.2f, 0.2f);
//        // setting these for a test
//        glUniform3f(glGetUniformLocation(reg_shader, "light.position"), 0.f, 0.f, 0.f);
//        glUniform3f(glGetUniformLocation(reg_shader, "light.diffuse"), 0.7f, 0.7f, 0.7f);
//        glUniform3f(glGetUniformLocation(reg_shader, "light.specular"), 0.2f, 0.2f, 0.2f);
//        for (const auto &wall_pos : wall_positions) {
//            // calc model matrix for every wall
//            glm::mat4 model = glm::mat4(1.0f);
//            model = wall_pos;
//
//            glUniformMatrix4fv(glGetUniformLocation(reg_shader, "pv"), 1, GL_FALSE, glm::value_ptr(proj * view));
//            glUniformMatrix4fv(glGetUniformLocation(reg_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
//            glBindVertexArray(wall.vao);
//
//            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//        }
//
//        // draw roof/floor
//        for (const auto &roof_pos : roof_positions) {
//            // calc model matrix for every wall
//            glm::mat4 model = glm::mat4(1.0f);
//            model = roof_pos;
//
//            glUniformMatrix4fv(glGetUniformLocation(reg_shader, "pv"), 1, GL_FALSE, glm::value_ptr(proj * view));
//            glUniformMatrix4fv(glGetUniformLocation(reg_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
//            glBindVertexArray(roof.vao);
//
//            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//        }
//
//        // draw light cube
//        // TODO: translate??
//        glm::mat4 model = glm::mat4(1.0f);
//
//        glUseProgram(light_shader);
//        glUniformMatrix4fv(glGetUniformLocation(light_shader, "mvp"), 1, GL_FALSE, glm::value_ptr(proj * view * model));
//        glBindVertexArray(light.vao);
//
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//    }
}
