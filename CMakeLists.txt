cmake_minimum_required(VERSION 3.12)

project("Simple Shooter")
set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(glad REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib)
find_package(glfw3 REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib)
find_package(glm REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib)
find_package(stb REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib)
find_package(chicken3421 REQUIRED HINTS ${PROJECT_SOURCE_DIR}/lib)

set(LIBS glad::glad glm::glm glfw stb chicken3421)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

include(copy_resources)
copy_resources(${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)

add_executable(simple_shooter src/camera.cpp src/primitives.cpp include/primitives.hpp include/camera.hpp)
target_include_directories(simple_shooter PUBLIC include)
target_sources(simple_shooter
        ${target}
        PRIVATE
        include/utils.hpp
        include/primitives.hpp
        include/camera.hpp

        src/main.cpp
        src/utils.cpp
        src/primitives.cpp
        src/camera.cpp
        )
target_link_libraries(simple_shooter PUBLIC ${LIBS})
