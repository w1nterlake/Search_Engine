cmake_minimum_required(VERSION 3.30)
project(search_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

set(PROJECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

include(FetchContent)

FetchContent_Declare(
  nlohmann_json
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/include/json/json-develop"
)
FetchContent_MakeAvailable(nlohmann_json)

add_subdirectory(src)
add_subdirectory(Test)
