/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021, 2022, 2023 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif // DOXYGEN_SHOUÖD_SKIP_THIS

#include "EchoServerContextFactory.h"

#include <core/SNodeC.h>
#include <net/in/stream/legacy/SocketServer.h>
//
#include <log/Logger.h>
//
#include <string>

int main(int argc, char* argv[]) {
    core::SNodeC::init(argc, argv); // Initialize the framework.
                                    // Configure logging, create command line
                                    // arguments for named instances.

    using EchoServer = net::in::stream::legacy::SocketServer<EchoServerContextFactory>; // Simplify data type
                                                                                        // Note the use of our implemented
                                                                                        // EchoServerContextFactory as
                                                                                        // template argument
    using SocketAddress = EchoServer::SocketAddress;                                    // Simplify data type

    EchoServer echoServer; // Create server instance and listen on all interfaces on port 8001
    echoServer.listen(8001, 5, [](const SocketAddress& socketAddress, const core::socket::State& state) -> void {
        switch (state) {
            case core::socket::State::OK:
                VLOG(1) << "EchoServer: connected to '" << socketAddress.toString() << "'";
                break;
            case core::socket::State::DISABLED:
                VLOG(1) << "EchoServer: disabled";
                break;
            case core::socket::State::ERROR:
                VLOG(1) << "EchoServer: " << socketAddress.toString() << ": " << state.what();
                break;
            case core::socket::State::FATAL:
                VLOG(1) << "EchoServer: " << socketAddress.toString() << ": " << state.what();
                break;
        }
    });

    return core::SNodeC::start(); // Start the event loop, daemonize if requested.
}
