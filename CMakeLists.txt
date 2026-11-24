cmake_minimum_required(VERSION 3.15)
project(sylar)

include(cmake/utils.cmake)

set(CMAKE_VERSION_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")

include_directories(.)

set(LIB_SRC
    sylar/address.cc
    sylar/config.cc
    sylar/hook.cc
    sylar/fd_manager.cc
    sylar/fiber.cc
    sylar/iomanager.cc
    sylar/log.cc
    sylar/mutex.cc
    sylar/scheduler.cc
    sylar/socket.cc
    sylar/thread.cc
    sylar/timer.cc
    sylar/util.cc
)

find_package(yaml-cpp REQUIRED)

add_library(sylar SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(sylar)  #__FILE__
target_link_libraries(sylar PUBLIC yaml-cpp)
#add_library(sylar_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(sylar_static PROPERTIES OUTPUT_NAME "sylar")
# target_include_directories(sylar PUBLIC ${PROJECT_SOURCE_DIR})

# set(LIB_LIB
#     sylar
#     pthread
# )

add_executable(test_log tests/test_log.cc)
add_dependencies(test_log sylar)
force_redefine_file_macro_for_sources(test_log)  #__FILE__
target_link_libraries(test_log sylar)

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config sylar)
force_redefine_file_macro_for_sources(test_config)  #__FILE__
target_link_libraries(test_config sylar)

add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread sylar)
force_redefine_file_macro_for_sources(test_thread)  #__FILE__
target_link_libraries(test_thread sylar)

add_executable(test_util tests/test_util.cc)
add_dependencies(test_util sylar)
force_redefine_file_macro_for_sources(test_util)  #__FILE__
target_link_libraries(test_util sylar)

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber sylar)
force_redefine_file_macro_for_sources(test_fiber)  #__FILE__
target_link_libraries(test_fiber sylar)

add_executable(test_ucontext tests/test_ucontext.cc)
add_dependencies(test_ucontext sylar)
force_redefine_file_macro_for_sources(test_ucontext)  #__FILE__
target_link_libraries(test_ucontext sylar)

add_executable(test_scheduler tests/test_scheduler.cc)
add_dependencies(test_scheduler sylar)
force_redefine_file_macro_for_sources(test_scheduler)  #__FILE__
target_link_libraries(test_scheduler sylar)

add_executable(test_ranklist tests/test_ranklist.cc)
add_dependencies(test_ranklist sylar)
force_redefine_file_macro_for_sources(test_ranklist)  #__FILE__
target_link_libraries(test_ranklist sylar)

add_executable(test_iomanager tests/test_iomanager.cc)
add_dependencies(test_iomanager sylar)
force_redefine_file_macro_for_sources(test_iomanager)  #__FILE__
target_link_libraries(test_iomanager sylar)

add_executable(test_hook tests/test_hook.cc)
add_dependencies(test_hook sylar)
force_redefine_file_macro_for_sources(test_hook)  #__FILE__
target_link_libraries(test_hook sylar)

add_executable(test_address tests/test_address.cc)
add_dependencies(test_address sylar)
force_redefine_file_macro_for_sources(test_address)  #__FILE__
target_link_libraries(test_address sylar)

add_executable(test_socket tests/test_socket.cc)
add_dependencies(test_socket sylar)
force_redefine_file_macro_for_sources(test_socket)  #__FILE__
target_link_libraries(test_socket sylar)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)