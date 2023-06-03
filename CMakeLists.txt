cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ViewFactor VERSION 1.0)

set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Add Libraries
add_subdirectory(${source_path}/graphics)
add_subdirectory(${source_path}/monte-carlo)
add_subdirectory(${source_path}/parser)

# Add the executable
add_executable(${PROJECT_NAME} src/main.cpp)
add_executable(${PROJECT_NAME}_Test src/mainTest.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC Graphics MonteCarlo Parser)
target_link_libraries(${PROJECT_NAME}_Test PUBLIC Graphics MonteCarlo Parser)