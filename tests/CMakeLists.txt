cmake_minimum_required(VERSION 3.0.0)

find_package(CURL REQUIRED)
list(APPEND Tests status usertouuid usertouuids uuidtons uuidtoprofile blockedservers statistics)
set(Tests ${Tests} PARENT_SCOPE)

foreach(test IN LISTS Tests)
    add_executable(${test} ${test}.c)
    target_include_directories(${test} PUBLIC ${CMAKE_SOURCE_DIR}/include .)
    target_link_libraries(${test} PUBLIC curl)
endforeach()