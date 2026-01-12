#include <GL/glew.h>
#include <GL/glext.h>
#include <cstddef>

#include "Mesh.h"

Mesh::Mesh(std::vector<Types::Vertex> &vertices, std::vector<Types::Texture> &textures, std::vector<unsigned> &indices) {
  this->vertices = vertices;
  this->textures = textures;
  this->indices = indices;
  this->setupMesh();
}

void Mesh::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &IBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Types::Vertex), &vertices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Types::Vertex), reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Types::Vertex), reinterpret_cast<void *>(offsetof(Types::Vertex, normal)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Types::Vertex), reinterpret_cast<void *>(offsetof(Types::Vertex, textureCoordinates)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void Mesh::draw(Shader &shader) {
  unsigned diffuse = 1, specular = 1, normal = 1, height = 1;
  for (int i = 0; i < textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    std::string number;
    std::string name = textures[i].type;
    if (name == "textureDiffuse") {
      number = std::to_string(diffuse++);
    } else if (name == "textureSpecular") {
      number = std::to_string(specular++);
    }
    std::string uniformName = name + number;
    shader.setInt(uniformName, i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glActiveTexture(GL_TEXTURE0);
}
