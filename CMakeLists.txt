cmake_minimum_required (VERSION 3.8)

project ("AstMarker")

find_package(imgui CONFIG REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)


add_executable (AstMarker "AstMarker.cpp" "AstMarker.h")
target_link_libraries(AstMarker PRIVATE imgui::imgui)
target_link_libraries(AstMarker PRIVATE unofficial::nativefiledialog::nfd)
target_link_libraries(AstMarker PRIVATE ${OpenCV_LIBS})


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AstMarker PROPERTY CXX_STANDARD 20)
endif()
