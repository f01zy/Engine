#include <GL/glew.h>
#include <ext/matrix_transform.hpp>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include "Objects.h"

Objects::Objects() {}
void Objects::addObject(const Object &object, const unsigned shaderId) { objects[shaderId].push_back(object); }

void Objects::render(ResourceManager &resourceManager, unsigned matricesUBO, glm::mat4 &view, glm::mat4 &projection) {
  glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glm::mat4 model;
  for (auto iter = objects.begin(); iter != objects.end(); iter++) {
    Shader &shader = resourceManager.getShaderById(iter->first);
    shader.use();
    for (const Object &object : iter->second) {
      if (object.renderFlags & SEPARATE_MATRICES) {
        shader.setMat4("projection", projection);
        shader.setMat4("view", object.renderFlags & NO_TRANSFORM ? glm::mat4(glm::mat3(view)) : view);
      }
      model = glm::mat4(1.0f);
      model = glm::translate(model, object.position);
      shader.setMat4("model", model);
      if (object.textureTarget != 0 && object.texture != 0) {
        glBindTexture(object.textureTarget, object.texture);
      }
      if (object.renderFlags & DEPTH_TEST) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        if (object.renderFlags & DEPTH_LEQUAL) {
          glDepthFunc(GL_LEQUAL);
        }
      }
      if (object.renderFlags & CULL_FACE) {
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
