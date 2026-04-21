cmake_minimum_required(VERSION 4.3)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
project(
    interloq
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "Railway Interlocking Design library"
)
file(GLOB INTERLOQ_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx)
set(INTERLOQ_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(INTERLOQ_FKYAML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fkYAML/include)
set(
    INTERLOQ_MAGIC_ENUM_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/magic_enum/include
)

option(INTERLOQ_BUILD_TESTS "Build Unit Tests" ON)

add_library(${PROJECT_NAME} ${INTERLOQ_SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INTERLOQ_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${INTERLOQ_FKYAML_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${INTERLOQ_MAGIC_ENUM_DIR})

if(INTERLOQ_BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()
