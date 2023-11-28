cmake_minimum_required (VERSION 3.8)

project ("astMarker")

find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)
find_package(glad REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)



add_executable(astMarker
        "main.cpp"
        "image.cpp"
        "image.h"
        "program.cpp"
        "program.h")

target_link_libraries(astMarker
        ${OpenCV_LIBS}
        ${OPENGL_LIBRARIES}
        imgui::imgui
        glad::glad
        unofficial::nativefiledialog::nfd
        )


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET astMarker PROPERTY CXX_STANDARD 20)
endif()
