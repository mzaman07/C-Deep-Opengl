cmake_minimum_required(VERSION 3.20.0)
project(3dCgraphics VERSION 0.1.0 LANGUAGES C)
cmake_policy(SET CMP0072 NEW)
add_subdirectory(thirdParty/glfw-3.4)
add_subdirectory(thirdParty/glad)
add_subdirectory(thirdParty/cglm)

message(STATUS "COMpiler=${CMAKE_C_COMPILER}")

# include_directories(libs/include/)
# add this to the cmake folder to module path
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# message(PREMOD=${GLFW3_INCLUDE_DIR})
# include_directories(3dCgrahics ${GLFW3_INCLUDE_DIR})
message(MOD=${CMAKE_SOURCE_DIR}/src/components)
# find_package(OpenGL REQUIRED)
# target_link_directories(3dCgraphics PUBLIC ${CMAKE_SOURCE_DIR}/libs/lib-vc2022/)

add_executable(3dCgraphics src/main.c src/components/shader.c)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src/components")

# message(Var=${GLFW3_LIBRARY})
# target_link_libraries(3dCgraphics ${GLFW3_LIBRARY})
target_link_libraries(3dCgraphics PRIVATE glfw glad cglm)



include(CTest)
enable_testing()

