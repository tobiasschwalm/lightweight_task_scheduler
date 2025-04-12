cmake_minimum_required(VERSION 3.10)
project(LTS)

# Set the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF) # Enforce ANSI-C compliance

# Enable all warnings and pedantic mode
add_compile_options(-Wall -Wextra -Werror -pedantic)

# Directories
set(LIB_DIR lib)
set(LIB_SRC_DIR ${LIB_DIR}/src)
set(LIB_INC_DIR ${LIB_DIR}/include)
set(TEST_DIR tests)
set(EXAMPLE_DIR example)
set(EXAMPLE_SRC_DIR ${EXAMPLE_DIR}/src)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Include directories
include_directories(${LIB_INC_DIR})

# Library target
file(GLOB LIB_SOURCES ${LIB_SRC_DIR}/*.c)
add_library(liblts STATIC ${LIB_SOURCES})
set_target_properties(liblts PROPERTIES OUTPUT_NAME lts)

# Example targets
set(EXAMPLE_FILES
    example/src/example_periodic_task.c
    example/src/example_one_time_task.c
    example/src/example_stop_scheduler.c
    example/src/example_stop_task.c
    example/src/example_error_handling.c
)

foreach(EXAMPLE_FILE ${EXAMPLE_FILES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_FILE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_FILE} example/src/platform_time.c)
    target_link_libraries(${EXAMPLE_NAME} liblts)
    target_include_directories(${EXAMPLE_NAME} PRIVATE ${LIB_INC_DIR})
endforeach()

# Test target
file(GLOB TEST_SOURCES ${TEST_DIR}/*.c)
add_executable(test_scheduler ${TEST_SOURCES})
target_link_libraries(test_scheduler liblts)

# Enable testing
enable_testing()
add_test(NAME RunTests COMMAND test_scheduler)
