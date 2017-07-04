/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin-client.
 *
 * libbitcoin-client is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_CLIENT_MESSAGE_STREAM_HPP
#define LIBBITCOIN_CLIENT_MESSAGE_STREAM_HPP

#include <bitcoin/bitcoin.hpp>

namespace libbitcoin {
namespace client {

/**
 * Represents a stream of multi-part messages.
 *
 * One of this library's design goals is completely separate the networking
 * code from the message-handling code. This interface is the glue between
 * the two worlds.
 */
class message_stream
{
public:
    virtual ~message_stream() {};

    /**
     * Sends one multi-part message.
     */
    virtual void write(const data_stack& data) = 0;
};

} // namespace client
} // namespace libbitcoin

#endif

