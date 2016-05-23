cmake_minimum_required(VERSION 3.5)
project(cquiz)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(SERVER_SOURCE_FILES
        src/server/main.c
        src/common/list.c
        src/common/list.h
        src/common/questions.c
        src/common/questions.h
        src/server/question_parser.c
        src/server/question_parser.h)

add_executable(cquiz-server ${SERVER_SOURCE_FILES})
configure_file(questions.md questions.md COPYONLY)