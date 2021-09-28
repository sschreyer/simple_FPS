set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/example_proj)

copy_resources(${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)

include_directories(include)

# In bigger projects a dedicated CMakeLists.txt would be in the `src` folder that does this
add_executable(example_proj)
target_sources(example_proj PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)
target_link_libraries(example_proj PUBLIC ${COMMON_LIBS})

