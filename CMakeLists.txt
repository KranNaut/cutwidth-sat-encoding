cmake_minimum_required(VERSION 3.22)

project(FIND_CUTWIDTH
        VERSION 1.0.0
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

include(AddGitSubmodule)
set(DSTATICCOMPILE ON CACHE BOOL "Cryptominisat Statically linked libs")
set(DIPASIR OFF CACHE BOOL "Build Cryptominisat Interface")
add_git_submodule(external/cryptominisat)

set(EXECUTABLE_NAME "Cutwidth")
message("Executable set")

add_subdirectory(src)
add_subdirectory(app)



