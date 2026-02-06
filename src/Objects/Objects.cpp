#include <GL/glew.h>
#include <ext/matrix_transform.hpp>
#include <glm.hpp>

#include "Objects.h"

Objects::Objects() {}
void Objects::addObject(Object &object, unsigned shaderId) { objects[shaderId].push_back(object); }

void Objects::draw(std::unordered_map<unsigned, Shader &> shaders) {
  glm::mat4 model;
  for (auto iter = objects.begin(); iter != objects.end(); iter++) {
    Shader &shader = shaders[iter->first];
    shader.use();
    for (const Object &object : iter->second) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, object.position);
      shader.setMat4("model", model);
      if (object.textureTarget != 0 && object.texture != 0) {
        glBindTexture(object.textureTarget, object.texture);
      }
      if (object.renderFlags & GL_DEPTH) {
        glEnable(GL_DEPTH_TEST);
        if (object.renderFlags & GL_LESS) {
          glDepthFunc(GL_LESS);
        } else if (object.renderFlags & GL_LEQUAL) {
          glDepthFunc(GL_LEQUAL);
        }
      }
      if (object.renderFlags & GL_CULL_FACE) {
        glEnable(GL_CULL_FACE);
      }
      glBindVertexArray(object.vertexArray);
      glDrawArrays(GL_TRIANGLES, 0, object.verticesCount);
      glBindVertexArray(0);
      glDisable(GL_CULL_FACE);
      glDisable(GL_DEPTH_TEST);
    }
  }
}
