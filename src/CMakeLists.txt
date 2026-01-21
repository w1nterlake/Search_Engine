
add_library(search_engine_core
    ConverterJSON.cpp
    InvertedIndex.cpp
    SearchServer.cpp
)

target_include_directories(search_engine_core
    PUBLIC
        ${PROJECT_INCLUDE_DIR}
)

target_link_libraries(search_engine_core
    PUBLIC
        nlohmann_json::nlohmann_json
)

find_package(Threads REQUIRED)
target_link_libraries(search_engine_core PUBLIC Threads::Threads)

add_executable(search_engine
    main.cpp
)

target_link_libraries(search_engine PRIVATE search_engine_core)
