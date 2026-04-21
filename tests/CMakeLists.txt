Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1
)
FetchContent_MakeAvailable(Catch2)
file(GLOB INTERLOQ_TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
foreach( TEST ${INTERLOQ_TEST_FILES} )
    get_filename_component( BINARY_NAME ${TEST} NAME_WLE )
    add_executable( ${BINARY_NAME} ${TEST} )
    target_link_libraries( ${BINARY_NAME} PUBLIC ${PROJECT_NAME} )
    target_link_libraries( ${BINARY_NAME} PRIVATE Catch2::Catch2WithMain )
    target_include_directories( ${BINARY_NAME} PUBLIC ${INTERLOQ_INCLUDE_DIR} )
    target_compile_definitions(
        ${BINARY_NAME}
        PRIVATE
        TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
    )
    catch_discover_tests(${BINARY_NAME} DISCOVERY_MODE PRE_TEST)
endforeach()

