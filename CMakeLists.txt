cmake_minimum_required(VERSION 3.5)
project(test_game_functions)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

set(SOURCE_FILES main.c
        game.c
        game.h
        players.h
        players.c)
add_executable(test_game_functions ${SOURCE_FILES})