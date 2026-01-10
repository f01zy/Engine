cmake_minimum_required(VERSION 3.5) 
project("OpenGL 3D Engine" VERSION 0.0.1) 

set(SOURCE 
  ./src/main.cpp 
  ./src/Service/Service.cpp 
  ./src/Shader/Shader.cpp 
  ./src/Texture/Texture.cpp 
  ./src/Camera/Camera.cpp 
  ./src/Lighting/Lighting.cpp
  ./src/Mesh/Mesh.cpp
) 
add_executable(main ${SOURCE})

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(./third_party/glfw)
add_subdirectory(./third_party/assimp)

target_include_directories(main PRIVATE
  ./third_party/assimp/include
  ./third_party/glm/glm
  ./third_party/stb
)

target_link_libraries(main PRIVATE 
  OpenGL::GL
  GLEW::GLEW
  glfw
  assimp
)
