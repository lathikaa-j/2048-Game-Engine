cmake_minimum_required(VERSION 3.10)
project(Game2048Engine LANGUAGES CXX)

add_executable(game2048
    main.cpp
    src/core/Tile.cpp
    src/core/Board.cpp
)

target_include_directories(game2048 PRIVATE include)
set_target_properties(game2048 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
