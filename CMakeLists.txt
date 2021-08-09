cmake_minimum_required(VERSION 3.0.0)
project(mojang-api-c VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(CURL REQUIRED)

add_library(mojang-api INTERFACE)
target_include_directories(mojang-api INTERFACE include/)
target_link_libraries(mojang-api INTERFACE curl)

add_subdirectory(tests)
foreach(test IN LISTS Tests)
    add_test(NAME ${test} COMMAND ${test})
endforeach()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
