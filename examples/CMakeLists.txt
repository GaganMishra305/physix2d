cmake_minimum_required(VERSION 3.10)
project(physix2d_examples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML REQUIRED sfml-all>=2.4)

# Point to parent library
add_subdirectory(.. ${CMAKE_BINARY_DIR}/physix2d_build)
target_include_directories(physix2d PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

# Auto-detect all .cpp files in current directory
file(GLOB DEMO_SOURCES "src/*.cpp")
# Create executables for each .cpp file
foreach(demo_source ${DEMO_SOURCES})
    get_filename_component(demo_name ${demo_source} NAME_WE)
    add_executable(${demo_name} ${demo_source})
    target_link_libraries(${demo_name} PRIVATE
        physix2d
        ${SFML_LIBRARIES}
    )
    
    # Include SFML headers
    target_include_directories(${demo_name} PRIVATE ${SFML_INCLUDE_DIRS})
    target_compile_options(${demo_name} PRIVATE ${SFML_CFLAGS_OTHER})
    message(STATUS "Building demo: ${demo_name}")
endforeach()

add_custom_target(all_demos)
foreach(demo_source ${DEMO_SOURCES})
    get_filename_component(demo_name ${demo_source} NAME_WE)
    add_dependencies(all_demos ${demo_name})
endforeach()