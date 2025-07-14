set(LIBRARY_SOURCES
    "encoding.cpp"
    "read_graph.cpp")

set(LIBRARY_HEADERS
    "encoding.h"
    "read_graph.h")

add_library(InternalLibraries STATIC 
            ${LIBRARY_SOURCES}
            ${LIBRARY_HEADERS})

target_link_libraries(InternalLibraries PUBLIC
                      cryptominisat5)

target_include_directories(InternalLibraries PUBLIC
                           "./")
