cmake_minimum_required(VERSION 3.15)
project(sylar)

set(CMAKE_VERSION_MAKEFILE ON)
set(CMAKE_CXX_FALGS "$ENV{CXXFLAGS} -rdynamic -03 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

set(LIB_SRC
    sylar/log.cc
)

add_library(sylar SHARED ${LIB_SRC})
#add_library(sylar_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(sylar_static PROPERTIES OUTPUT_NAME "sylar")
# target_include_directories(sylar PUBLIC ${PROJECT_SOURCE_DIR})

add_executable(test_log tests/test_log.cc)
add_dependencies(test_log sylar)
target_link_libraries(test_log sylar)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)