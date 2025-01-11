cmake_minimum_required(VERSION 3.31.3)

project(Server)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/src/lib/include)
link_directories(${CMAKE_SOURCE_DIR}/src/lib)


add_executable(server 
  src/main.cpp
  src/handlerRequest.cpp
  src/PostgreSQL.cpp
  
)
