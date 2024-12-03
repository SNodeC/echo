cmake_minimum_required(VERSION 3.3)

project(echo-nit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(iwyu)

find_package(snodec COMPONENTS net-in-stream-tls)

set(ECHOSERVER_CPP EchoServer.cpp)
set(ECHOSERVER_H EchoServerContextFactory.h EchoServerContext.h)

add_executable(echoserver-nit ${ECHOSERVER_CPP} ${ECHOSERVER_H})
target_link_libraries(echoserver-nit PRIVATE snodec::net-in-stream-tls)
target_compile_definitions(echoserver-nit PRIVATE CERT_PATH="${CMAKE_SOURCE_DIR}/certs/")

set(ECHOCLIENT_CPP EchoClient.cpp)
set(ECHOCLIENT_H EchoClientContextFactory.h EchoClientContext.h)

add_executable(echoclient-nit ${ECHOCLIENT_CPP} ${ECHOCLIENT_H})
target_link_libraries(echoclient-nit PRIVATE snodec::net-in-stream-tls)
target_compile_definitions(echoclient-nit PRIVATE CERT_PATH="${CMAKE_SOURCE_DIR}/certs/")

install(TARGETS echoserver-nit echoclient-nit RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
