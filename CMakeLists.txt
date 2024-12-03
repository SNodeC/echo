cmake_minimum_required(VERSION 3.18)

project(echo-mim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(iwyu)

find_package(snodec COMPONENTS net-in-stream-legacy net-in6-stream-legacy net-un-stream-legacy net-rc-stream-legacy net-l2-stream-legacy)

set(ECHOSERVER_CPP EchoServer.cpp)
set(ECHOSERVER_H EchoServerContextFactory.h EchoServerContext.h)

add_executable(echoserver-mim ${ECHOSERVER_CPP} ${ECHOSERVER_H})
target_link_libraries(echoserver-mim PRIVATE snodec::net-in-stream-legacy snodec::net-in6-stream-legacy snodec::net-un-stream-legacy snodec::net-rc-stream-legacy snodec::net-l2-stream-legacy)

set(ECHOCLIENT_CPP EchoClient.cpp)
set(ECHOCLIENT_H EchoClientContextFactory.h EchoClientContext.h)

add_executable(echoclient-mim ${ECHOCLIENT_CPP} ${ECHOCLIENT_H})
target_link_libraries(echoclient-mim PRIVATE snodec::net-in-stream-legacy snodec::net-in6-stream-legacy snodec::net-un-stream-legacy snodec::net-rc-stream-legacy snodec::net-l2-stream-legacy)

install(TARGETS echoserver-mim echoclient-mim RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
