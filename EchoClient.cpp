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
#include <net/in/stream/tls/SocketClient.h>
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

    using EchoClient = net::in::stream::tls::SocketClient<EchoClientContextFactory>; // Simplify data type
                                                                                     // Note the use of our implemented
                                                                                     // EchoClientContextFactory as
                                                                                     // template argument
    using SocketAddress = EchoClient::SocketAddress;                                 // Simplify data type
    using SocketConnection = EchoClient::SocketConnection;                           // Simplify data type

    EchoClient echoClient("echo"); // Create named client instance

    echoClient.getConfig().setReuseAddress();
    echoClient.getConfig().setCaCert(CERT_PATH "SNode.C-Server-CA.crt");    // Has to be in PEM format
    echoClient.getConfig().setCert(CERT_PATH "SNode.C-Client-Cert.pem");    // Has to be in PEM format
    echoClient.getConfig().setCertKey(CERT_PATH "SNode.C-Client-Cert.key"); // Has to be in PEM format
    echoClient.getConfig().setCertKeyPassword("snode.c");
    echoClient.getConfig().setSni("localhost");

    echoClient.setOnConnect([&echoClient](SocketConnection* socketConnection) -> void { // onConnect
        VLOG(0) << "OnConnect " << echoClient.getConfig().getInstanceName();

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

    echoClient.setOnConnected([&echoClient](SocketConnection* socketConnection) -> void { // onConnected
        VLOG(0) << "OnConnected " << echoClient.getConfig().getInstanceName();

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

    echoClient.setOnDisconnect([&echoClient](SocketConnection* socketConnection) -> void { // onDisconnect
        VLOG(0) << "OnDisconnect " << echoClient.getConfig().getInstanceName();

        VLOG(0) << "\tLocal: " + socketConnection->getLocalAddress().toString();
        VLOG(0) << "\tPeer:  " + socketConnection->getRemoteAddress().toString();
    });

    echoClient.connect( //
        "localhost",
        8001,
        [instanceName = echoClient.getConfig().getInstanceName()](const SocketAddress& socketAddress,
                                                                  const core::socket::State& state) -> void { // Connect to server
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
