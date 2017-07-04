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
#ifndef LIBBITCOIN_CLIENT_OBELISK_OBELISK_TYPES_HPP
#define LIBBITCOIN_CLIENT_OBELISK_OBELISK_TYPES_HPP

#include <bitcoin/bitcoin.hpp>

namespace libbitcoin {
namespace client {

struct history_row
{
    output_point output;
    size_t output_height;
    uint64_t value;
    input_point spend;
    size_t spend_height;
};

typedef std::vector<history_row> history_list;

struct stealth_row
{
    data_chunk ephemkey;
    payment_address address;
    hash_digest transaction_hash;
};

typedef std::vector<stealth_row> stealth_list;

// See below for description of updates data format.
enum class subscribe_type : uint8_t
{
    address = 0,
    stealth = 1
};

}
}

#endif
