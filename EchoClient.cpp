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
//
#include <net/in/stream/legacy/SocketClient.h>
#include <net/in6/stream/legacy/SocketClient.h>
#include <net/l2/stream/legacy/SocketClient.h>
#include <net/rc/stream/legacy/SocketClient.h>
#include <net/un/stream/legacy/SocketClient.h>
//
#include <log/Logger.h>
//
#include <string>

int main(int argc, char* argv[]) {
    core::SNodeC::init(argc, argv);

    using EchoClientIn = net::in::stream::legacy::SocketClient<EchoClientContextFactory, std::string>;
    using SocketAddressIn = EchoClientIn::SocketAddress;

    const EchoClientIn echoClientIn("in", "IPv4 Socket");
    echoClientIn.connect("localhost",
                         8001,
                         [instanceName = echoClientIn.getConfig().getInstanceName()](const SocketAddressIn& socketAddress,
                                                                                     const core::socket::State& state) -> void {
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

    using EchoClientIn6 = net::in6::stream::legacy::SocketClient<EchoClientContextFactory, std::string>;
    using SocketAddressIn6 = EchoClientIn6::SocketAddress;

    const EchoClientIn6 echoClientIn6("in6", "IPv6 Socket");
    echoClientIn6.connect("localhost",
                          8001,
                          [instanceName = echoClientIn6.getConfig().getInstanceName()](const SocketAddressIn6& socketAddress,
                                                                                       const core::socket::State& state) -> void {
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

    using EchoClientUn = net::un::stream::legacy::SocketClient<EchoClientContextFactory, std::string>;
    using SocketAddressUn = EchoClientUn::SocketAddress;

    const EchoClientUn echoClientUn("un", "Unix-Domain Socket");
    echoClientUn.connect("/tmp/echoserver",
                         [instanceName = echoClientUn.getConfig().getInstanceName()](const SocketAddressUn& socketAddress,
                                                                                     const core::socket::State& state) -> void {
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

    using EchoClientRc = net::rc::stream::legacy::SocketClient<EchoClientContextFactory, std::string>;
    using SocketAddressRc = EchoClientRc::SocketAddress;

    const EchoClientRc echoClientRc("rc", "RFCOMM Socket");
    echoClientRc.connect("44:01:BB:A3:63:32", // Bluetooth address of peer (MPOW)
                         1,
                         "70:D8:23:5B:B6:C4", // Bluetooth address (INTERN)
                         [instanceName = echoClientRc.getConfig().getInstanceName()](const SocketAddressRc& socketAddress,
                                                                                     const core::socket::State& state) -> void {
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

    using EchoClientL2 = net::l2::stream::legacy::SocketClient<EchoClientContextFactory, std::string>;
    using SocketAddressL2 = EchoClientL2::SocketAddress;

    const EchoClientL2 echoClientL2("l2", "L2CAP Socket");
    echoClientL2.connect("44:01:BB:A3:63:32", // Bluetooth address of peer (MPOW)
                         0x1023,
                         "70:D8:23:5B:B6:C4", // Bluetooth address (INTERN)
                         [instanceName = echoClientL2.getConfig().getInstanceName()](const SocketAddressL2& socketAddress,
                                                                                     const core::socket::State& state) -> void {
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
