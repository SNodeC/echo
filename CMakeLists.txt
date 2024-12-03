cmake_minimum_required(VERSION 3.3)

project(echo-main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(iwyu)

find_package(snodec COMPONENTS net-in-stream-legacy)

set(ECHOSERVER_CPP EchoServer.cpp)
set(ECHOSERVER_H EchoServerContextFactory.h EchoServerContext.h)

add_executable(echoserver-main ${ECHOSERVER_CPP} ${ECHOSERVER_H})
target_link_libraries(echoserver-main PRIVATE snodec::net-in-stream-legacy)

set(ECHOCLIENT_CPP EchoClient.cpp)
set(ECHOCLIENT_H EchoClientContextFactory.h EchoClientContext.h)

add_executable(echoclient-main ${ECHOCLIENT_CPP} ${ECHOCLIENT_H})
target_link_libraries(echoclient-main PRIVATE snodec::net-in-stream-legacy)

install(TARGETS echoserver-main echoclient-main RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
