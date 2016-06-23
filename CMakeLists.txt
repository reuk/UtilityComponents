cmake_minimum_required(VERSION 3.0)

project(UTILITY_COMPONENTS VERSION 0.1 LANGUAGES CXX)

set(JUCE_ROOT "" CACHE PATH "")
set(JUCE_MODULES "" CACHE PATH "")

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -Wall -Werror -std=c++14 ")

file(GLOB_RECURSE sources "*.cpp")
file(GLOB_RECURSE headers "*.hpp")

set(name utility-components)

add_library(${name} STATIC ${sources} ${headers})

target_include_directories(${name}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${JUCE_ROOT}
    ${JUCE_MODULES}
)
