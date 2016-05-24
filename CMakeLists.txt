cmake_minimum_required(VERSION 3.5)
project(cquiz)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -Wall")

set(SERVER_SOURCE_FILES
        src/server/main.c
        src/common/list.c
        src/common/list.h
        src/common/questions.c
        src/common/questions.h
        src/server/question_parser/question_parser.c
        src/server/question_parser/question_parser.h src/server/server_utils/server_utils.c src/server/server_utils/server_utils.h src/server/connection_handler/connection_handler.c src/server/connection_handler/connection_handler.h src/server/results/results.c src/server/results/results.h src/server/parsing_utils/parsing_utils.c src/server/parsing_utils/parsing_utils.h)

add_executable(cquiz-server ${SERVER_SOURCE_FILES})
configure_file(questions.md questions.md COPYONLY)
configure_file(results.txt results.txt COPYONLY)