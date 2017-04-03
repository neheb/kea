// Copyright (C) 2017 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEST_SERVER_UNIX_SOCKET_H
#define TEST_SERVER_UNIX_SOCKET_H

#include <config.h>
#include <asiolink/interval_timer.h>
#include <asiolink/io_service.h>
#include <boost/shared_ptr.hpp>
#include <gtest/gtest.h>
#include <array>
#include <string>

namespace isc {
namespace asiolink {
namespace test {

/// @brief Provides unix domain socket functionality for unit tests.
class TestServerUnixSocket {
public:

    /// @brief Constructor.
    ///
    /// @param io_service IO service.
    /// @param socket_file_path Socket file path.
    /// @param test_timeout Test timeout in milliseconds.
    /// @param custom_response Custom response to be sent to the client.
    TestServerUnixSocket(IOService& io_service,
                         const std::string& socket_file_path,
                         const long test_timeout,
                         const std::string& custom_respons_ = "");

    /// @brief Creates and binds server socket.
    ///
    /// @param stop_after_count Number of received messages after which the
    /// IO service should be stopped.
    void bindServerSocket(const unsigned int stop_after_count = 1);

    /// @brief Server acceptor handler.
    ///
    /// @param ec Error code.
    void acceptHandler(const boost::system::error_code& ec);

    /// @brief Server read handler.
    ///
    /// @param ec Error code.
    /// @param bytes_transferred Number of bytes read.
    void readHandler(const boost::system::error_code& ec,
                     size_t bytes_transferred);

    /// @brief Callback function invoke upon test timeout.
    ///
    /// It stops the IO service and reports test timeout.
    void timeoutHandler();

private:

    /// @brief Asynchronously accept new connections.
    void accept();

    /// @brief IO service used by the tests.
    IOService& io_service_;

    /// @brief Server endpoint.
    boost::asio::local::stream_protocol::endpoint server_endpoint_;
    /// @brief Server acceptor.
    boost::asio::local::stream_protocol::acceptor server_acceptor_;

    /// @brief Server side unix domain socket.
    boost::asio::local::stream_protocol::socket server_socket_;

    /// @brief Receive buffer.
    std::array<char, 1024> raw_buf_;

    /// @brief Asynchronous timer service to detect timeouts.
    IntervalTimer test_timer_;

    /// @brief Holds custom response to be sent to the client.
    std::string custom_response_;

    /// @brief Number of messages received after which IO service gets stopped.
    unsigned int stop_after_count_;

    /// @brief Number of messages received so far.
    unsigned int read_count_;
};

/// @brief Pointer to the @ref TestServerUnixSocket.
typedef boost::shared_ptr<TestServerUnixSocket> TestServerUnixSocketPtr;

} // end of namespace isc::asiolink::test
} // end of namespace isc::asiolink
} // end of namespace isc

#endif // TEST_SERVER_UNIX_SOCKET_H