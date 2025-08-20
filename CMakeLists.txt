cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project("Helicopter4")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLUT REQUIRED)
find_package(glui CONFIG REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)

add_executable(Helicopter4)

add_subdirectory(ext/glad)

set_target_properties(Helicopter4
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_sources(Helicopter4 PRIVATE
    "src/main.cpp"
)

target_include_directories(Helicopter4 PRIVATE
    "include/"
)

target_link_libraries(Helicopter4 PRIVATE
    GLUT::GLUT
    glad
    glfw
    glm::glm
    glui::glui glui::glui_static
    OpenGL::GL
)

#add_custom_command(TARGET Helicopter4 POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E create_symlink
#        "${CMAKE_CURRENT_SOURCE_DIR}/shader"
#        "<TARGET_FILE_DIR:Helicopter4>/shader"
#    VERBATIM
#)