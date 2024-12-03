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

#ifndef _ECHOCLIENTCONTEXT_H
#define _ECHOCLIENTCONTEXT_H

#include <core/socket/SocketAddress.h>
#include <core/socket/stream/SocketConnection.h>
#include <core/socket/stream/SocketContext.h>
#include <log/Logger.h>
//
#include <string>

class EchoClientContext : public core::socket::stream::SocketContext {
public:
    using core::socket::stream::SocketContext::SocketContext;

private:
    void onConnected() override { // Called in case a connection has been established successfully.
        VLOG(1) << "Echo connected to " << getSocketConnection()->getRemoteAddress().toString();

        VLOG(1) << "Initiating data exchange";
        sendToPeer("Hello peer! It's nice talking to you"); // Initiate the ping-pong data exchange.
    }

    void onDisconnected() override { // Called in case the connection has been closed.
        VLOG(1) << "Echo disconnected from " << getSocketConnection()->getRemoteAddress().toString();
    }

    bool onSignal(int signum) override { // Called in case a signal has been received
        VLOG(1) << "Echo disconnected due to signal=" << signum;
        return true; // Echo should be terminated
    }

    std::size_t onReceivedFromPeer() override { // Called in case data have already been received by the framework
                                                // and thus are ready for preccessing.
        char junk[4096];

        std::size_t junkLen = readFromPeer(junk, 4096); // Fetch data.
                                                        // In case there are less than 4096 bytes available return at
                                                        // least that amount of data.
                                                        // In case more than 4096 bytes are available
                                                        // onReceivedFromPeer will be called again.
                                                        // No error can occure here.
        if (junkLen > 0) {
            VLOG(1) << "Data to reflect: " << std::string(junk, junkLen);
            sendToPeer(junk, junkLen); // Reflect the received data back to the server.
                                       // Out of memory is the only error which can occure here.
        }

        return junkLen; // Return the amount of data processed to the framework.
    }
};

#endif // _ECHOCLIENTCONTEXT_H
