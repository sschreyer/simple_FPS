#include <renderer.hpp>
#include <camera.hpp>
#include <primitives.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <utils.hpp>
#include <utility>
#include <locations.hpp>

renderer_t make_renderer(const glm::mat4 &projection) {
    // load shader
    GLuint vert_shader, frag_shader;
    vert_shader = utils::load_shader("res/shaders/basic_vert.glsl", GL_VERTEX_SHADER);
    frag_shader = utils::load_shader("res/shaders/basic_frag.glsl", GL_FRAGMENT_SHADER);

    // create and link our shader program
    GLuint reg_shader = utils::make_shader(vert_shader, frag_shader);
    glUseProgram(reg_shader);

    // TODO: delete shader here (i.e. clean up)

    return { projection,
             reg_shader
    };
}

void
draw(const renderer_t &renderer, const glm::mat4 &p, const glm::mat4 &v, const glm::mat4 &m, const primitives::mesh_t &mesh, GLuint diff_map) {

    // set the diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diff_map);


    glUniformMatrix4fv(glGetUniformLocation(renderer.program, "model"), 1, GL_FALSE, glm::value_ptr(m));
    glUniformMatrix4fv(glGetUniformLocation(renderer.program, "pv"), 1, GL_FALSE, glm::value_ptr(p * v));

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    // note: hardcoded for now
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void render(const renderer_t &renderer, const locations::scene_t &scene) {
    glEnable(GL_DEPTH_TEST);
    // TODO: turn on when my assets/primitives actually support it
//    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(renderer.program);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glm::mat4 p = renderer.projection;
    glm::mat4 v = camera::get_view(scene.cam);

    // set lighting params
    int i = 0;
    std::string start = "pointLights[";
    std::string end = "]";
    for (const point_light_t &light : scene.point_lights ) {
        std::string result = start + std::to_string(i) + end + ".position";
        glUniform3fv(glGetUniformLocation(renderer.program, result.data()), 1, glm::value_ptr(light.pos));
        result = start + std::to_string(i) + end + ".ambient";
        // note: hardcoded
        glUniform3fv(glGetUniformLocation(renderer.program, result.data()), 1, glm::value_ptr(glm::vec3(0.1,0.1,0.1)));
        result = start + std::to_string(i) + end + ".diffuse";
        glUniform3fv(glGetUniformLocation(renderer.program, result.data()), 1, glm::value_ptr(light.color));

//        result = start + std::to_string(i) + end + ".linear";
//        glUniform1f(glGetUniformLocation(renderer.program, result.data()), 0.1);




        i++;
    }


    std::stack<std::pair<const locations::node_t &, glm::mat4>> renderables{{{ scene.root, scene.root.transform }}};
    while (!renderables.empty()) {
        const auto[node, parent_tf] = renderables.top();  // this is called a structured binding
        renderables.pop();

        glm::mat4 xform = parent_tf * node.transform;

        if (node.kind == locations::node_t::STATIC_MESH) {
            draw(renderer, p, v, xform, node.mesh, node.maps[0]);
        } else if (node.kind == locations::node_t::LIGHT_MESH) {
            continue;
        }

        for (const locations::node_t &c: node.children) {
            renderables.push({ c, xform });
        }
    }

}