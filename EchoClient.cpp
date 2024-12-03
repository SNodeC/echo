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

#include "EchoClientContextFactory.h"

#include <core/SNodeC.h>
#include <log/Logger.h>
#include <net/in/stream/legacy/SocketClient.h>
#include <string>

int main(int argc, char* argv[]) {
    core::SNodeC::init(argc, argv); // Initialize the framework.
                                    // Configure logging, create command line
                                    // arguments for named instances.

    using EchoClient = net::in::stream::legacy::SocketClient<EchoClientContextFactory>; // Simplify data type
                                                                                        // Note the use of our implemented
                                                                                        // EchoClientContextFactory as
                                                                                        // template argument
    using SocketAddress = EchoClient::SocketAddress;                                    // Simplify data type

    EchoClient echoClient("echo"); // Create named client instance and connect to localhost:8001
    echoClient.connect("localhost",
                       8001,
                       [instanceName = echoClient.getConfig().getInstanceName()](
                           const SocketAddress& socketAddress, const core::socket::State& state) -> void { // Connect to server
                           switch (state) {
                               case core::socket::State::OK:
                                   VLOG(1) << instanceName << ": connected to '" << socketAddress.toString() << "'";
                                   break;
                               case core::socket::State::DISABLED:
                                   VLOG(1) << instanceName << ": disabled";
                                   break;
                               case core::socket::State::ERROR:
                                   LOG(ERROR) << instanceName << ": " << socketAddress.toString() << ": " << state.what();
                                   break;
                               case core::socket::State::FATAL:
                                   LOG(FATAL) << instanceName << ": " << socketAddress.toString() << ": " << state.what();
                                   break;
                           }
                       });

    return core::SNodeC::start(); // Start the event loop, daemonize if requested.
}
