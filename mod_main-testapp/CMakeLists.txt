cmake_minimum_required(VERSION 3.31.3)

project(MainServer)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(OpenSSL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/lib/include/openssl")
set(OpenSSL_LIBRARIES "${CMAKE_SOURCE_DIR}/src/lib/lib")


set(jwt-cpp_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/lib/include/jwt-cpp")


include_directories(${OpenSSL_INCLUDE_DIR} ${jwt-cpp_INCLUDE_DIR})


add_executable(main_server 
  src/main.cpp
  src/Client.cpp
  src/getAddress.cpp
  src/Token.cpp
)


target_link_libraries(main_server PRIVATE
  ${OpenSSL_LIBRARIES}/libcrypto.lib
  ${jwt-cpp_LIBRARIES}
)
