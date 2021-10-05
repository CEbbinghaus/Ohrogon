cmake_minimum_required(VERSION 3.10)

project(Ohrogon VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

file(COPY src/data/ DESTINATION data)

# find_package(OpenGL REQUIRED)
# include_directories(${OPENGL_INCLUDE_DIRS})
# if(OPENGL_FOUND)
#     message("Found OpenGL in the current environment!")
# else()
#     message("Error: No OpenGL found.")
# endif()

# message("OpenGL include dirs:\n[${OPENGL_INCLUDE_DIR}]" )
# link_libraries(${OPENGL_LIBRARIES})

add_library(Glad 
	lib/glad/src/glad.c
)

target_include_directories(Glad PUBLIC lib/glad/include)

include_directories(lib/glad/include/glad)
include_directories(lib/atyp)
# include_directories(lib/glcore)
include_directories(lib/glfw/includes)
include_directories(lib/stbi)

link_directories(lib/glfw/)

link_libraries(glfw3.lib)

file(GLOB SourceFiles src/**.cpp)
file(GLOB SourceHeaders src/**.h)
add_executable(Main ${SourceFiles} ${SourceHeaders})

target_link_libraries(Main Glad)

# target_include_directories(Main PUBLIC lib/glfw/include/)
# target_link_libraries(Main -Llib/glfw/)
# target_link_libraries(Main glfw.lib)