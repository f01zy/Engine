#include <GL/glew.h>
#include <cstddef>

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned> indices) {
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
  glBindBuffer(GL_VERTEX_ARRAY, VBO);
  glBufferData(GL_VERTEX_ARRAY, vertices.size() * sizeof(Vertex), &vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, textureCoordinates)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void Mesh::draw(Shader &shader) {
  unsigned diffuse = 1;
  unsigned specular = 1;
  for (int i = 0; i < textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    std::string number;
    std::string name = textures[i].type;
    if (name == "texture_diffuse") {
      number = std::to_string(diffuse++);
    } else if (name == "texture_specular") {
      number = std::to_string(specular++);
    }
    shader.setFloat(("material." + name + number).c_str(), i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
