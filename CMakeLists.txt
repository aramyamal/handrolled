cmake_minimum_required(VERSION 3.15...4.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(handrolled
    VERSION 0.0.1
    DESCRIPTION "Handrolled, type-safe and bounds-checked data structure implementations for C."
    LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BUILD_EXAMPLES "Build example programs in examples/" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries instead of static" OFF)
option(BUILD_TESTS "Build and enable tests" OFF)

# function to add compiler warnings
function(add_compiler_warnings_private target_name)
    target_compile_options(${target_name} PRIVATE
        $<$<C_COMPILER_ID:MSVC>:/W4>
        $<$<C_COMPILER_ID:GNU,Clang>:
            -Wall
            -Wextra
            -Wpedantic
            -Werror
            -Wmissing-prototypes
            -Wstrict-prototypes
        >
    )
endfunction()

add_library(handrolled 
    src/array.c
    src/vector.c
    src/hashmap.c
)

add_library(handrolled::handrolled ALIAS handrolled)

target_include_directories(handrolled PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# add compiler warnings to private handrolled
add_compiler_warnings_private(handrolled)

if(BUILD_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    enable_testing()

    function(add_test_executable target_name source_file)
        add_executable(${target_name} ${source_file})
        target_link_libraries(${target_name} PRIVATE handrolled::handrolled)
        add_compiler_warnings_private(${target_name})
        
        add_test(NAME ${target_name} COMMAND ${target_name})
    endfunction()

    set(TESTS
        "tests/test_array.c"
        "tests/test_vector.c"
        "tests/test_hashmap.c"
    )
    
    foreach(test_source ${TESTS})
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${test_source}")
            get_filename_component(test_name ${test_source} NAME_WE)
            add_test_executable(${test_name} ${test_source})
            message(STATUS "Added test: ${test_name}")
        else()
            message(WARNING "Test file not found: ${test_source}")
        endif()
    endforeach()

endif()

if(BUILD_EXAMPLES)
    function(add_example_executable target_name source_file)
        add_executable(${target_name} ${source_file})
        target_link_libraries(${target_name} PRIVATE handrolled::handrolled)
        add_compiler_warnings_private(${target_name})
    endfunction()

    set(EXAMPLES
        "examples/example_array.c"
        "examples/example_vector.c"
    )
    
     foreach(example_source ${EXAMPLES})
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${example_source}")
            get_filename_component(example_name ${example_source} NAME_WE)
            add_example_executable(${example_name} ${example_source})
            message(STATUS "Added example: ${example_name}")
        else()
            message(WARNING "Example file not found: ${example_source}")
        endif()
    endforeach()
endif()
