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
  bool isDepthTestEnable = false;
  bool isCullFaceEnabled = false;

  for (auto iter = objects.begin(); iter != objects.end(); iter++) {
    Shader &shader = resourceManager.getShaderById(iter->first);
    shader.use();

    for (const Object &object : iter->second) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, object.position);
      if (object.renderFlags & SCALE) {
        model = glm::scale(model, object.scale);
      }
      if (object.renderFlags & ROTATE) {
        model = glm::rotate(model, object.angle, object.rotate);
      }
      shader.setMat4("model", model);
      shader.setVec3("objectColor", object.objectColor);

      if (object.renderFlags & SEPARATE_MATRICES) {
        shader.setMat4("projection", projection);
        shader.setMat4("view", object.renderFlags & NO_TRANSFORM ? glm::mat4(glm::mat3(view)) : view);
      }

      if (object.textureTarget != 0 && object.texture != 0) {
        glBindTexture(object.textureTarget, object.texture);
      }

      if (object.renderFlags & DEPTH_TEST && !isDepthTestEnable) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        if (object.renderFlags & DEPTH_LEQUAL) {
          glDepthFunc(GL_LEQUAL);
        }
        isDepthTestEnable = true;
      } else if (!(object.renderFlags & DEPTH_TEST)) {
        glDisable(GL_DEPTH_TEST);
        isDepthTestEnable = false;
      }

      if (object.renderFlags & CULL_FACE && !isCullFaceEnabled) {
        glEnable(GL_CULL_FACE);
        isCullFaceEnabled = true;
      } else if (!(object.renderFlags & CULL_FACE)) {
        glDisable(GL_CULL_FACE);
        isCullFaceEnabled = false;
      }

      glBindVertexArray(object.vertexArray);
      glDrawArrays(GL_TRIANGLES, 0, object.verticesCount);
      glBindVertexArray(0);
    }
  }
}
