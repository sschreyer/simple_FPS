#include <primitives.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glfw/glfw3.h>

#include <vector>

#include <locations.hpp>
#include <model.hpp>

namespace locations {
    scene_t setup_room() {
        // make camera
        camera::camera_t cam = camera::make_camera(glm::vec3(0.f,0.f,1.5f), glm::vec3(0,0,0));

        node_t light_1 {
                primitives::make_light_cube(),
                STATIC_MESH,
                0,
                glm::translate(glm::mat4(1), {0, 0, 2}),
                {}
        };

        node_t light_2 {
                primitives::make_light_cube(),
                STATIC_MESH,
                0,
                glm::translate(glm::mat4(1), {10, 0, 2}),
                {}
        };

        node_t room {
                primitives::make_rect_prism(40,3,5),
                STATIC_MESH,
                0,
                glm::mat4(1),
                {light_1, light_2}
        };

        node_t root = {{},
                        EMPTY,
                        0,
                        glm::mat4(1),
                        {room}
        };

        std::vector<point_light_t> pointlights;
        pointlights.push_back({{0,0,2}, {0.5,0.5,0.5}, 0.2});
        pointlights.push_back({{10,0,2}, {0.5,0.5,0.5}, 0.2});

        return {cam, pointlights,  root};
    }
    // TODO: adapt or make new function for drawing nodes
//    void draw_starting_room(GLuint reg_shader, GLuint light_shader, glm::mat4 proj, glm::mat4 view, glm::vec3 cam_pos) {
//        // draw room
//        glUseProgram(reg_shader);
//
//        // light uniforms
//        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[0].ambient"), 0.15f, 0.15f, 0.15f);
//        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[0].position"), light_positions[0].x, light_positions[0].y, light_positions[0].z);
//        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[0].diffuse"), 0.5f, 0.5f, 0.5f);
//        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[0].specular"), 0.2f, 0.2f, 0.2f);
//        // light attenutation
//        glUniform1f(glGetUniformLocation(reg_shader, "pointLights[0].constant"), 1.0f);
//        glUniform1f(glGetUniformLocation(reg_shader, "pointLights[0].linear"), 0.22f);
//        glUniform1f(glGetUniformLocation(reg_shader, "pointLights[0].quadratic"), 0.20f);
//
//        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[1].ambient"), 0.15f, 0.15f, 0.15f);
//        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[1].position"), light_positions[1].x, light_positions[1].y, light_positions[1].z);
//        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[1].diffuse"), 0.5f, 0.5f, 0.5f);
//        glUniform3f(glGetUniformLocation(reg_shader, "pointLights[1].specular"), 0.2f, 0.2f, 0.2f);
//        glUniform1f(glGetUniformLocation(reg_shader, "pointLights[1].constant"), 1.0f);
//        glUniform1f(glGetUniformLocation(reg_shader, "pointLights[1].linear"), 0.22f);
//        glUniform1f(glGetUniformLocation(reg_shader, "pointLights[1].quadratic"), 0.20f);
//
//
//        glUniform3f(glGetUniformLocation(reg_shader, "camPos"), cam_pos.x, cam_pos.y, cam_pos.z);
//
//        glm::mat4 model = glm::mat4(1); //glm::translate(glm::mat4(1), glm::vec3(0, 1, 15)); //glm::mat4(1.0f);
//
//        glUniformMatrix4fv(glGetUniformLocation(reg_shader, "pv"), 1, GL_FALSE, glm::value_ptr(proj * view));
//        glUniformMatrix4fv(glGetUniformLocation(reg_shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
//        glBindVertexArray(room.vao);
//
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//        // draw light cube
//        for (auto light_pos : light_positions) {
//            model = glm::translate(glm::mat4(1), light_pos);
//            model *= glm::scale(glm::mat4(1), glm::vec3(0.3, 0.3, 0.3));
//
//            glUseProgram(light_shader);
//            glUniformMatrix4fv(glGetUniformLocation(light_shader, "mvp"), 1, GL_FALSE,
//                               glm::value_ptr(proj * view * model));
//            glBindVertexArray(light.vao);
//
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//        }
//    }
}
