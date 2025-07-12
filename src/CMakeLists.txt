set(LIBRARY_SOURCES
    "encoding.cpp"
    "read_graph.cpp")

set(LIBRARY_HEADERS
    "encoding.h"
    "read_graph.h")

add_library(InternalLibraries STATIC
            ${LIBRARY_SOURCES}
            ${LIBRARY_HEADERS})
