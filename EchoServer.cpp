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
#include <log/Logger.h>
#include <net/in/stream/legacy/SocketServer.h>
#include <net/rc/stream/legacy/SocketServer.h>
#include <net/un/stream/legacy/SocketServer.h>
#include <string>

int main(int argc, char* argv[]) {
    core::SNodeC::init(argc, argv);

    using EchoServerIn = net::in::stream::legacy::SocketServer<EchoServerContextFactory>;
    using SocketAddressIn = EchoServerIn::SocketAddress;

    EchoServerIn echoServerIn;
    echoServerIn.listen(8001, [](const SocketAddressIn& socketAddress, const core::socket::State& state) -> void {
        switch (state) {
            case core::socket::State::OK:
                VLOG(1) << "EchoServerIn: listening on '" << socketAddress.toString() << "'";
                break;
            case core::socket::State::DISABLED:
                VLOG(1) << "EchoServerIn: disabled";
                break;
            case core::socket::State::ERROR:
                VLOG(1) << "EchoServerIn: " << socketAddress.toString() << ": " << state.what();
                break;
            case core::socket::State::FATAL:
                VLOG(1) << "EchoServerIn: " << socketAddress.toString() << ": " << state.what();
                break;
        }
    });

    using EchoServerUn = net::un::stream::legacy::SocketServer<EchoServerContextFactory>;
    using SocketAddressUn = EchoServerUn::SocketAddress;

    EchoServerUn echoServerUn;
    echoServerUn.listen("/tmp/echoserver", [](const SocketAddressUn& socketAddress, const core::socket::State& state) -> void {
        switch (state) {
            case core::socket::State::OK:
                VLOG(1) << "EchoServerUn: listening on '" << socketAddress.toString() << "'";
                break;
            case core::socket::State::DISABLED:
                VLOG(1) << "EchoServerUn: disabled";
                break;
            case core::socket::State::ERROR:
                VLOG(1) << "EchoServerUn: " << socketAddress.toString() << ": " << state.what();
                break;
            case core::socket::State::FATAL:
                VLOG(1) << "EchoServerUn: " << socketAddress.toString() << ": " << state.what();
                break;
        }
    });

    using EchoServerRc = net::rc::stream::legacy::SocketServer<EchoServerContextFactory>;
    using SocketAddressRc = EchoServerRc::SocketAddress;

    // Disabled because a peer would be needed and that's out of scope of this demo app.
    // If a peer exists and runs the echo server remove the commented line
    // echoClientRc.getConfig().setDisabled() and specify the bluetooth address and channel
    // of the peer.
    EchoServerRc echoServerRc;
    echoServerRc.getConfig().setDisabled();
    echoServerRc.listen(16, [](const SocketAddressRc& socketAddress, const core::socket::State& state) -> void {
        switch (state) {
            case core::socket::State::OK:
                VLOG(1) << "EchoServerRc: listening on '" << socketAddress.toString() << "'";
                break;
            case core::socket::State::DISABLED:
                VLOG(1) << "EchoServerRc: disabled";
                break;
            case core::socket::State::ERROR:
                VLOG(1) << "EchoServerRc: " << socketAddress.toString() << ": " << state.what();
                break;
            case core::socket::State::FATAL:
                VLOG(1) << "EchoServerRc: " << socketAddress.toString() << ": " << state.what();
                break;
        }
    });

    return core::SNodeC::start(); // Start the event loop, daemonize if requested.
}
