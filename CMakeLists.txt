cmake_minimum_required(VERSION 2.4)
set(CMAKE_CXX_STANDARD 20)
project(nes_emulator)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/)

file(GLOB SOURCES src/*.cpp src/**/*.cpp)

add_executable(${PROJECT_NAME}.exe ${SOURCES})

target_include_directories(${PROJECT_NAME}.exe PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ ${CMAKE_CURRENT_SOURCE_DIR}/include/cpu)

target_compile_options(${PROJECT_NAME}.exe PRIVATE -Werror -Wall -Wextra)
target_link_libraries(${PROJECT_NAME}.exe -lpthread)