cmake_minimum_required (VERSION 3.8)

project ("astMarker")


find_package(OpenCV REQUIRED)
find_package(imgui REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)




add_executable(astMarker
        "main.cpp"
        "image.cpp"
        "image.h"
        )

target_link_libraries(astMarker
        ${OpenCV_LIBS}
        imgui::imgui
        unofficial::nativefiledialog::nfd
        sfml-system sfml-network sfml-graphics sfml-window
        ImGui-SFML::ImGui-SFML
        )


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET astMarker PROPERTY CXX_STANDARD 20)
endif()
