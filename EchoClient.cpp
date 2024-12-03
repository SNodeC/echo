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
#include <net/rc/stream/legacy/SocketClient.h>
#include <net/un/stream/legacy/SocketClient.h>
#include <string>

int main(int argc, char* argv[]) {
    core::SNodeC::init(argc, argv);

    static std::string ipv4("IPv4 Socket");
    using EchoClientIn = net::in::stream::legacy::SocketClient<EchoClientContextFactory, std::string>;
    using SocketAddressIn = EchoClientIn::SocketAddress;

    EchoClientIn echoClientIn("IPv4 Socket"); // This is not a named instance. Instead the SocketContextFactory expects a std::string. Thus,
                                              // "IPv4 Socket" is passed as argument to it.
    echoClientIn.connect("localhost", 8001, [](const SocketAddressIn& socketAddress, const core::socket::State& state) -> void {
        switch (state) {
            case core::socket::State::OK:
                VLOG(1) << "EchoClientIn: connected to '" << socketAddress.toString() << "'";
                break;
            case core::socket::State::DISABLED:
                VLOG(1) << "EchoClientIn: disabled";
                break;
            case core::socket::State::ERROR:
                LOG(ERROR) << "EchoClientIn: " << socketAddress.toString() << ": " << state.what();
                break;
            case core::socket::State::FATAL:
                LOG(FATAL) << "EchoClientIn: " << socketAddress.toString() << ": " << state.what();
                break;
        }
    });

    static std::string unixDomain("Unix-Domain Socket");
    using EchoClientUn = net::un::stream::legacy::SocketClient<EchoClientContextFactory, std::string>;
    using SocketAddressUn = EchoClientUn::SocketAddress;

    EchoClientUn echoClientUn("Unix-Domain Socket"); // This is not a named instance. Instead the SocketContextFactory expects a
                                                     // std::string. Thus, "Unix-Domain Socket" is passed as argument to it.
    echoClientUn.connect("/tmp/echoserver", [](const SocketAddressUn& socketAddress, const core::socket::State& state) -> void {
        switch (state) {
            case core::socket::State::OK:
                VLOG(1) << "EchoClientUn: : connected to '" << socketAddress.toString() << "'";
                break;
            case core::socket::State::DISABLED:
                VLOG(1) << "EchoClientUn: disabled";
                break;
            case core::socket::State::ERROR:
                LOG(ERROR) << "EchoClientUn: " << socketAddress.toString() << ": " << state.what();
                break;
            case core::socket::State::FATAL:
                LOG(FATAL) << "EchoClientUn: " << socketAddress.toString() << ": " << state.what();
                break;
        }
    });

    // Disabled because a peer would be needed and that's out of scope of this demo app.
    // If a peer exists and runs the echo server remove the commented line
    // echoClientRc.getConfig().setDisabled(); and specify the bluetooth address and channel
    // of the peer.
    static std::string rc("RFCOMM Socket");
    using EchoClientRc = net::rc::stream::legacy::SocketClient<EchoClientContextFactory, std::string>;
    using SocketAddressRc = EchoClientRc::SocketAddress;

    EchoClientRc echoClientRc("RFCOMM Socket"); // This is not a named instance. Instead the SocketContextFactory expects a std::string.
                                                // Thus, "RFCOMM Socket" is passed as argument to it.
    echoClientRc.getConfig().setDisabled();
    echoClientRc.connect("10:3D:1C:AC:BA:9C", 1, [](const SocketAddressRc& socketAddress, const core::socket::State& state) -> void {
        switch (state) {
            case core::socket::State::OK:
                VLOG(1) << "EchoClientRc: connected to '" << socketAddress.toString() << "'";
                break;
            case core::socket::State::DISABLED:
                VLOG(1) << "EchoClientRc: disabled";
                break;
            case core::socket::State::ERROR:
                LOG(ERROR) << "EchoClientRc: " << socketAddress.toString() << ": " << state.what();
                break;
            case core::socket::State::FATAL:
                LOG(FATAL) << "EchoClientRc: " << socketAddress.toString() << ": " << state.what();
                break;
        }
    });

    return core::SNodeC::start(); // Start the event loop, daemonize if requested.
}
