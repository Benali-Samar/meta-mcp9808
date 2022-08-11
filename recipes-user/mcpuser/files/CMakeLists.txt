cmake_minimum_required  (VERSION 3.15)

#setting the project name
project (mcp9808)

#specifiting the c++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

#specifiying the executable file.cpp/.c
add_executable (mcpuser mcpuser.cpp)

# install binary
install (TARGETS mcpuser DESTINATION bin)


