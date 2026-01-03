cmake_minimum_required(VERSION 3.5) 
project("OpenGL 3D Engine" VERSION 0.0.1) 

set(SOURCE ./src/main.cpp ./src/Service/Service.cpp ./src/Shader/Shader.cpp ./src/Camera/Camera.cpp) 
add_executable(main ${SOURCE})

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(third_party/glfw)

target_link_libraries(main PRIVATE OpenGL::GL)
target_link_libraries(main PRIVATE GLEW::GLEW)
target_link_libraries(main PRIVATE glfw)
