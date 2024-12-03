cmake_minimum_required(VERSION 3.3)

project(echo-mi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(iwyu)

find_package(snodec COMPONENTS net-in-stream-legacy net-un-stream-legacy net-rc-stream-legacy)

set(ECHOSERVER_CPP EchoServer.cpp)
set(ECHOSERVER_H EchoServerContextFactory.h EchoServerContext.h)

add_executable(echoserver-mi ${ECHOSERVER_CPP} ${ECHOSERVER_H})
target_link_libraries(echoserver-mi PRIVATE snodec::net-in-stream-legacy snodec::net-un-stream-legacy snodec::net-rc-stream-legacy)

set(ECHOCLIENT_CPP EchoClient.cpp)
set(ECHOCLIENT_H EchoClientContextFactory.h EchoClientContext.h)

add_executable(echoclient-mi ${ECHOCLIENT_CPP} ${ECHOCLIENT_H})
target_link_libraries(echoclient-mi PRIVATE snodec::net-in-stream-legacy snodec::net-un-stream-legacy snodec::net-rc-stream-legacy)

install(TARGETS echoserver-mi echoclient-mi RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
