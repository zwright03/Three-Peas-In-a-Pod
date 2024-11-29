# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.8)
project(DSA-Project-3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable (DSA-Project-3 "DSA-Project-3/src/main.cpp" 
                "DSA-Project-3/src/timer.cpp" 
                "DSA-Project-3/src/timer.h" 
                "DSA-Project-3/src/DataPoint.cpp" 
                "DSA-Project-3/src/DataPoint.h"
                "DSA-Project-3/src/SortShape.cpp"
                "DSA-Project-3/src/SortShape.h"
                "DSA-Project-3/src/SortVisualizer.cpp"
                "DSA-Project-3/src/SortVisualizer.h"
                "DSA-Project-3/src/MergeVisualizer.h"
                "DSA-Project-3/src/MergeVisualizer.cpp"
                "DSA-Project-3/src/QuickVisualizer.cpp"
                "DSA-Project-3/src/QuickVisualizer.h")


target_link_libraries(DSA-Project-3 PRIVATE sfml-graphics)
target_compile_features(DSA-Project-3 PRIVATE cxx_std_17)

add_custom_command(TARGET DSA-Project-3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/DSA-Project-3/resources"
    "$<TARGET_FILE_DIR:DSA-Project-3>/resources"
)

if(WIN32)
    add_custom_command(
        TARGET DSA-Project-3
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:DSA-Project-3>
        VERBATIM)
endif()