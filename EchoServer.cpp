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
#include <net/in/stream/tls/SocketServer.h>
#include <string>
//
#include <cstddef>           // for size_t
#include <openssl/asn1.h>    // for ASN1_STRING_get0_data, ASN1_STRING_length
#include <openssl/crypto.h>  // for OPENSSL_free
#include <openssl/obj_mac.h> // for NID_subject_alt_name
#include <openssl/ssl.h>
#include <openssl/types.h> // for X509
#include <openssl/x509.h> // for X509_NAME_oneline, X509_free, X509_get_ext_d2i, X509_get_issuer_name, X509_get_subject_name, X509_verify_cert_error_string
#include <openssl/x509v3.h>
#include <stdint.h> // for int32_t

int main(int argc, char* argv[]) {
    core::SNodeC::init(argc, argv); // Initialize the framework.
                                    // Configure logging, create command line
                                    // arguments for named instances.

    using EchoServer = net::in::stream::tls::SocketServer<EchoServerContextFactory>; // Simplify data type
                                                                                     // Note the use of our implemented
                                                                                     // EchoServerContextFactory as
                                                                                     // template argument
    using SocketAddress = EchoServer::SocketAddress;                                 // Simplify data type
    using SocketConnection = EchoServer::SocketConnection;                           // Simplify data type

    EchoServer echoServer("echo"); // Create named server instance

    echoServer.getConfig().setReuseAddress();
    echoServer.getConfig().setCaCert(CERT_PATH "SNode.C-Client-CA.crt");    // Has to be in PEM format
    echoServer.getConfig().setCert(CERT_PATH "SNode.C-Server-Cert.pem");    // Has to be in PEM format
    echoServer.getConfig().setCertKey(CERT_PATH "SNode.C-Server-Cert.key"); // Has to be in PEM format
    echoServer.getConfig().setCertKeyPassword("snode.c");

    echoServer.setOnConnect([&echoServer](SocketConnection* socketConnection) -> void { // onConnect
        VLOG(0) << "OnConnect " << echoServer.getConfig().getInstanceName();

        VLOG(0) << "\tLocal: " + socketConnection->getLocalAddress().toString();
        VLOG(0) << "\tPeer:  " + socketConnection->getRemoteAddress().toString();

        // From here on it's "only" OpenSSL certificate handling!

        /* Enable automatic hostname checks */
        // X509_VERIFY_PARAM* param = SSL_get0_param(socketConnection->getSSL());

        // X509_VERIFY_PARAM_set_hostflags(param, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS);
        // if (!X509_VERIFY_PARAM_set1_host(param, "localhost", sizeof("localhost") - 1)) {
        //   // handle error
        //   socketConnection->close();
        // }
    });

    echoServer.setOnConnected([&echoServer](SocketConnection* socketConnection) -> void { // onConnected
        VLOG(0) << "OnConnected " << echoServer.getConfig().getInstanceName();

        // From here on it's "only" OpenSSL certificate handling!
        X509* server_cert = SSL_get_peer_certificate(socketConnection->getSSL());
        if (server_cert != nullptr) {
            long verifyErr = SSL_get_verify_result(socketConnection->getSSL());

            VLOG(0) << "\tPeer certificate: " + std::string(X509_verify_cert_error_string(verifyErr));

            char* str = X509_NAME_oneline(X509_get_subject_name(server_cert), nullptr, 0);
            VLOG(0) << "\t   Subject: " + std::string(str);
            OPENSSL_free(str);

            str = X509_NAME_oneline(X509_get_issuer_name(server_cert), nullptr, 0);
            VLOG(0) << "\t   Issuer: " + std::string(str);
            OPENSSL_free(str);

            // We could do all sorts of certificate verification stuff here before deallocating the certificate.

            GENERAL_NAMES* subjectAltNames =
                static_cast<GENERAL_NAMES*>(X509_get_ext_d2i(server_cert, NID_subject_alt_name, nullptr, nullptr));

            int32_t altNameCount = sk_GENERAL_NAME_num(subjectAltNames);

            VLOG(0) << "\t   Subject alternative name count: " << altNameCount;
            for (int32_t i = 0; i < altNameCount; ++i) {
                GENERAL_NAME* generalName = sk_GENERAL_NAME_value(subjectAltNames, i);

                if (generalName->type == GEN_URI) {
                    std::string subjectAltName =
                        std::string(reinterpret_cast<const char*>(ASN1_STRING_get0_data(generalName->d.uniformResourceIdentifier)),
                                    static_cast<std::size_t>(ASN1_STRING_length(generalName->d.uniformResourceIdentifier)));
                    VLOG(0) << "\t      SAN (URI): '" + subjectAltName + "'";
                } else if (generalName->type == GEN_DNS) {
                    std::string subjectAltName = std::string(reinterpret_cast<const char*>(ASN1_STRING_get0_data(generalName->d.dNSName)),
                                                             static_cast<std::size_t>(ASN1_STRING_length(generalName->d.dNSName)));
                    VLOG(0) << "\t      SAN (DNS): '" + subjectAltName + "'";
                } else {
                    VLOG(0) << "\t      SAN (Type): '" + std::to_string(generalName->type) + "'";
                }
            }
            sk_GENERAL_NAME_pop_free(subjectAltNames, GENERAL_NAME_free);

            X509_free(server_cert);
        } else {
            VLOG(0) << "\tPeer certificate: no certificate";
        }
    });

    echoServer.setOnDisconnect([&echoServer](SocketConnection* socketConnection) -> void { // onDisconnect
        VLOG(0) << "OnDisconnect " << echoServer.getConfig().getInstanceName();

        VLOG(0) << "\tLocal: " + socketConnection->getLocalAddress().toString();
        VLOG(0) << "\tPeer:  " + socketConnection->getRemoteAddress().toString();
    });

    echoServer.listen( // Listen on port 8001 on all interfaces
        8001,
        [instanceName = echoServer.getConfig().getInstanceName()](const SocketAddress& socketAddress,
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
