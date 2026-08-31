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
#include <SemanticLog.h>
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
                snode::semantic::appLog().trace() << "EchoServerIn: listening on '" << socketAddress.toString() << "'";
                break;
            case core::socket::State::DISABLED:
                snode::semantic::appLog().trace() << "EchoServerIn: disabled";
                break;
            case core::socket::State::ERROR:
                snode::semantic::appLog().trace() << "EchoServerIn: " << socketAddress.toString() << ": " << state.what();
                break;
            case core::socket::State::FATAL:
                snode::semantic::appLog().trace() << "EchoServerIn: " << socketAddress.toString() << ": " << state.what();
                break;
        }
    });

    using EchoServerUn = net::un::stream::legacy::SocketServer<EchoServerContextFactory>;
    using SocketAddressUn = EchoServerUn::SocketAddress;

    EchoServerUn echoServerUn;
    echoServerUn.listen("/tmp/echoserver", [](const SocketAddressUn& socketAddress, const core::socket::State& state) -> void {
        switch (state) {
            case core::socket::State::OK:
                snode::semantic::appLog().trace() << "EchoServerUn: listening on '" << socketAddress.toString() << "'";
                break;
            case core::socket::State::DISABLED:
                snode::semantic::appLog().trace() << "EchoServerUn: disabled";
                break;
            case core::socket::State::ERROR:
                snode::semantic::appLog().trace() << "EchoServerUn: " << socketAddress.toString() << ": " << state.what();
                break;
            case core::socket::State::FATAL:
                snode::semantic::appLog().trace() << "EchoServerUn: " << socketAddress.toString() << ": " << state.what();
                break;
        }
    });

    using EchoServerRc = net::rc::stream::legacy::SocketServer<EchoServerContextFactory>;
    using SocketAddressRc = EchoServerRc::SocketAddress;

    // Disabled because a peer would be needed and that's out of scope of this demo app.
    // If a peer exists and runs the echo server remove the commented line
    // echoClientRc.getConfig()->setDisabled() and specify the bluetooth address and channel
    // of the peer.
    EchoServerRc echoServerRc;
    echoServerRc.getConfig()->setDisabled();
    echoServerRc.listen(16, [](const SocketAddressRc& socketAddress, const core::socket::State& state) -> void {
        switch (state) {
            case core::socket::State::OK:
                snode::semantic::appLog().trace() << "EchoServerRc: listening on '" << socketAddress.toString() << "'";
                break;
            case core::socket::State::DISABLED:
                snode::semantic::appLog().trace() << "EchoServerRc: disabled";
                break;
            case core::socket::State::ERROR:
                snode::semantic::appLog().trace() << "EchoServerRc: " << socketAddress.toString() << ": " << state.what();
                break;
            case core::socket::State::FATAL:
                snode::semantic::appLog().trace() << "EchoServerRc: " << socketAddress.toString() << ": " << state.what();
                break;
        }
    });

    return core::SNodeC::start(); // Start the event loop, daemonize if requested.
}
