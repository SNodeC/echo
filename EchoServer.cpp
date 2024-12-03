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
//
#include <net/in/stream/legacy/SocketServer.h>
#include <net/in6/stream/legacy/SocketServer.h>
#include <net/l2/stream/legacy/SocketServer.h>
#include <net/rc/stream/legacy/SocketServer.h>
#include <net/un/stream/legacy/SocketServer.h>
//
#include <log/Logger.h>
//
#include <string>

int main(int argc, char* argv[]) {
    core::SNodeC::init(argc, argv);

    using EchoServerIn = net::in::stream::legacy::SocketServer<EchoServerContextFactory>;
    using SocketAddressIn = EchoServerIn::SocketAddress;

    const EchoServerIn echoServerIn("in");
    echoServerIn.getConfig().setReuseAddress();
    echoServerIn.listen( //
        8001,
        [instanceName = echoServerIn.getConfig().getInstanceName()](const SocketAddressIn& socketAddress,
                                                                    const core::socket::State& state) -> void {
            switch (state) {
                case core::socket::State::OK:
                    VLOG(1) << instanceName << ": listening on '" << socketAddress.toString() << "'";
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

    using EchoServerIn6 = net::in6::stream::legacy::SocketServer<EchoServerContextFactory>;
    using SocketAddressIn6 = EchoServerIn6::SocketAddress;

    const EchoServerIn6 echoServerIn6("in6");
    echoServerIn6.getConfig().setIPv6Only();
    echoServerIn6.getConfig().setReuseAddress();
    echoServerIn6.listen( //
        8001,
        [instanceName = echoServerIn6.getConfig().getInstanceName()](const SocketAddressIn6& socketAddress,
                                                                     const core::socket::State& state) -> void {
            switch (state) {
                case core::socket::State::OK:
                    VLOG(1) << "in6"
                            << ": listening on '" << socketAddress.toString() << "'";
                    break;
                case core::socket::State::DISABLED:
                    VLOG(1) << "in6"
                            << ": disabled";
                    break;
                case core::socket::State::ERROR:
                    LOG(ERROR) << "in6"
                               << ": " << socketAddress.toString() << ": " << state.what();
                    break;
                case core::socket::State::FATAL:
                    LOG(FATAL) << "in6"
                               << ": " << socketAddress.toString() << ": " << state.what();
                    break;
            }
        });

    using EchoServerUn = net::un::stream::legacy::SocketServer<EchoServerContextFactory>;
    using SocketAddressUn = EchoServerUn::SocketAddress;

    const EchoServerUn echoServerUn("un");
    echoServerUn.listen( //
        "/tmp/echoserver",
        [instanceName = echoServerUn.getConfig().getInstanceName()](const SocketAddressUn& socketAddress,
                                                                    const core::socket::State& state) -> void {
            switch (state) {
                case core::socket::State::OK:
                    VLOG(1) << instanceName << ": listening on '" << socketAddress.toString() << "'";
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

    using EchoServerRc = net::rc::stream::legacy::SocketServer<EchoServerContextFactory>;
    using SocketAddressRc = EchoServerRc::SocketAddress;

    const EchoServerRc echoServerRc("rc");
    echoServerRc.listen( //
        "44:01:BB:A3:63:32",
        1,
        [instanceName = echoServerRc.getConfig().getInstanceName()](const SocketAddressRc& socketAddress,
                                                                    const core::socket::State& state) -> void {
            switch (state) {
                case core::socket::State::OK:
                    VLOG(1) << instanceName << ": listening on '" << socketAddress.toString() << "'";
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

    using EchoServerL2 = net::l2::stream::legacy::SocketServer<EchoServerContextFactory>;
    using SocketAddressL2 = EchoServerL2::SocketAddress;

    const EchoServerL2 echoServerL2("l2");
    echoServerL2.listen( //
        "44:01:BB:A3:63:32",
        0x1023,
        [instanceName = echoServerL2.getConfig().getInstanceName()](const SocketAddressL2& socketAddress,
                                                                    const core::socket::State& state) -> void {
            switch (state) {
                case core::socket::State::OK:
                    VLOG(1) << instanceName << ": listening on '" << socketAddress.toString() << "'";
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
