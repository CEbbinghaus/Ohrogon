cmake_minimum_required(VERSION 3.10)

project(Ohrogon VERSION 0.0.1)

set(CWD ${CMAKE_SOURCE_DIR})
set(SourceDir ${CWD}/src)
set(LibraryDir ${CWD}/lib)
set(DataDir ${CWD}/data)

set(CMAKE_CXX_STANDARD 17)

set(buildDirectory obj)

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
	${LibraryDir}/glad/src/glad.c
)

target_include_directories(Glad PUBLIC ${LibraryDir}/glad/include)

include_directories(${LibraryDir}/glad/include/glad)
include_directories(${LibraryDir}/atyp)
# include_directories(${LibraryDir}/glcore)
include_directories(${LibraryDir}/glfw/includes)
include_directories(${LibraryDir}/stbi)

link_directories(${LibraryDir}/glfw/)

link_libraries(glfw3.lib)

file(GLOB SourceFiles ${SourceDir}/**.cpp)
file(GLOB SourceHeaders ${SourceDir}/**.h)

add_executable(Main ${SourceFiles} ${SourceHeaders})

target_link_libraries(Main Glad)

# target_include_directories(Main PUBLIC ${LibraryDir}/glfw/include/)
# target_link_libraries(Main -L${LibraryDir}/glfw/)
# target_link_libraries(Main glfw.lib)