add_executable(${EXECUTABLE_NAME} "main.cpp")
target_link_libraries(${EXECUTABLE_NAME} PUBLIC 
                      InternalLibraries
                      cryptominisat5)
