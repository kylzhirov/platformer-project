cmake_minimum_required(VERSION 3.27)
project(platformer)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib CONFIG REQUIRED)
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fsanitize=address -fsanitize=undefined")
endif()

add_executable(platformer platformer.cpp globals.h graphics.h level.h player.h enemy.h enemy_manager.cpp enemy_manager.h assets.h utilities.h
        player.cpp
        level_manager.cpp
        game.cpp
        game.h
        graphics.cpp
        assets.cpp
        utilities.cpp)

target_link_libraries(platformer PRIVATE raylib)
