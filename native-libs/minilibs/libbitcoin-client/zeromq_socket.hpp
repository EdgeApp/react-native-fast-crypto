/*
 * Copyright (c) 2011-2014 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_CLIENT_SOCKET_HPP
#define LIBBITCOIN_CLIENT_SOCKET_HPP

#include <zmq.h>
#include "message_stream.hpp"

namespace libbitcoin {
namespace client {

/**
 * Represents a connection to the bitcoin server.
 */
class BC_API zeromq_socket
  : public message_stream
{
public:
    BC_API ~zeromq_socket();
    BC_API zeromq_socket(void* context, int type=ZMQ_DEALER);

    /**
     * Connects to a remote server.
     * @param key optional key for encrypted servers.
     * @return false if something went wrong. The socket will be unusable
     * in that case.
     */
    BC_API bool connect(const std::string& address, const std::string& key="");

    /**
     * Begins listening for incoming connections.
     * @return false if something went wrong. The socket will be unusable
     * in that case.
     */
    BC_API bool bind(const std::string& address);

    /**
     * Creates a zeromq pollitem_t structure suitable for passing to the
     * zmq_poll function. When zmq_poll indicates that there is data waiting
     * to be read, simply call the `forward` method to handle it.
     */
    BC_API zmq_pollitem_t pollitem();

    /**
     * Forwards pending input messages to the given message_stream interface.
     */
    BC_API bool forward(message_stream& dest);

    /**
     * Forwards pending input messages to the given zeromq socket in a
     * zero-copy manner.
     */
    BC_API bool forward(zeromq_socket& dest);

    // message_stream interface:
    virtual void write(const data_stack& data);

private:
    /**
     * Returns true if there are pending input messages.
     */
    bool pending();

    void* socket_;
};

} // namespace client
} // namespace libbitcoin

#endif


