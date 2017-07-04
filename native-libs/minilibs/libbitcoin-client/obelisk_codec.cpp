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
#include "obelisk_codec.hpp"

namespace libbitcoin {
namespace client {

using std::placeholders::_1;

/**
 * Reverses data.
 * Due to an unfortunate historical accident,
 * the obelisk wire format puts address hashes in reverse order.
 */
template <typename T>
T reverse(const T& in)
{
    T out;
    std::reverse_copy(in.begin(), in.end(), out.begin());
    return out;
}

obelisk_codec::obelisk_codec(
    std::shared_ptr<message_stream> out,
    update_handler on_update,
    unknown_handler on_unknown,
    period_ms timeout,
    uint8_t retries)
  : obelisk_router(out)
{
    set_on_update(std::move(on_update));
    set_on_unknown(std::move(on_unknown));
    set_timeout(timeout);
    set_retries(retries);
}

obelisk_codec::~obelisk_codec()
{
}

void obelisk_codec::fetch_history(error_handler on_error,
    fetch_history_handler on_reply,
    const payment_address& address, uint32_t from_height)
{
    auto data = build_data({
        to_byte(address.version()),
        reverse(address.hash()),
        to_little_endian<uint32_t>(from_height)
    });

    send_request("blockchain.fetch_history", data, std::move(on_error),
        std::bind(decode_fetch_history, _1, std::move(on_reply)));
}

void obelisk_codec::fetch_transaction(error_handler on_error,
    fetch_transaction_handler on_reply,
    const hash_digest& tx_hash)
{
    auto data = build_data({
        tx_hash
    });

    send_request("blockchain.fetch_transaction", data, std::move(on_error),
        std::bind(decode_fetch_transaction, _1, std::move(on_reply)));
}

void obelisk_codec::fetch_last_height(error_handler on_error,
    fetch_last_height_handler on_reply)
{
    auto data = data_chunk();

    send_request("blockchain.fetch_last_height", data,
        std::move(on_error),
        std::bind(decode_fetch_last_height, _1, std::move(on_reply)));
}

void obelisk_codec::fetch_block_header(error_handler on_error,
    fetch_block_header_handler on_reply,
    uint32_t height)
{
    auto data = build_data({
        to_little_endian<uint32_t>(height)
    });

    send_request("blockchain.fetch_block_header", data, std::move(on_error),
        std::bind(decode_fetch_block_header, _1, std::move(on_reply)));
}

void obelisk_codec::fetch_block_header(error_handler on_error,
    fetch_block_header_handler on_reply,
    const hash_digest& blk_hash)
{
    auto data = build_data({
        blk_hash
    });

    send_request("blockchain.fetch_block_header", data, std::move(on_error),
        std::bind(decode_fetch_block_header, _1, std::move(on_reply)));
}

void obelisk_codec::fetch_transaction_index(error_handler on_error,
    fetch_transaction_index_handler on_reply,
    const hash_digest& tx_hash)
{
    auto data = build_data({
        tx_hash
    });

    send_request("blockchain.fetch_transaction_index", data,
        std::move(on_error),
        std::bind(decode_fetch_transaction_index, _1, std::move(on_reply)));
}

void obelisk_codec::fetch_stealth(error_handler on_error,
    fetch_stealth_handler on_reply,
    const binary_type& prefix, uint32_t from_height)
{
    // should this be a throw or should there be a return type instead?
    if (prefix.size() > max_uint8)
    {
        on_error(std::make_error_code(std::errc::bad_address));
        return;
    }

    auto data = build_data({
        to_byte(static_cast<uint8_t>(prefix.size())),
        prefix.blocks(),
        to_little_endian<uint32_t>(from_height)
    });

    send_request("blockchain.fetch_stealth", data, std::move(on_error),
        std::bind(decode_fetch_stealth, _1, std::move(on_reply)));
}

void obelisk_codec::validate(error_handler on_error,
    validate_handler on_reply,
    const transaction_type& tx)
{
    data_chunk data(satoshi_raw_size(tx));
    DEBUG_ONLY(auto it =) satoshi_save(tx, data.begin());
    BITCOIN_ASSERT(it == data.end());

    send_request("transaction_pool.validate", data, std::move(on_error),
        std::bind(decode_validate, _1, std::move(on_reply)));
}

void obelisk_codec::fetch_unconfirmed_transaction(
    error_handler on_error,
    fetch_transaction_handler on_reply,
    const hash_digest& tx_hash)
{
    auto data = build_data({
        tx_hash
    });

    send_request("transaction_pool.fetch_transaction", data,
        std::move(on_error),
        std::bind(decode_fetch_transaction, _1, std::move(on_reply)));
}

void obelisk_codec::broadcast_transaction(error_handler on_error,
    empty_handler on_reply,
    const transaction_type& tx)
{
    data_chunk data(satoshi_raw_size(tx));
    DEBUG_ONLY(auto it =) satoshi_save(tx, data.begin());
    BITCOIN_ASSERT(it == data.end());

    send_request("protocol.broadcast_transaction", data, std::move(on_error),
        std::bind(decode_empty, _1, std::move(on_reply)));
}

void obelisk_codec::address_fetch_history(error_handler on_error,
    fetch_history_handler on_reply,
    const payment_address& address, uint32_t from_height)
{
    auto data = build_data({
        to_byte(address.version()),
        reverse(address.hash()),
        to_little_endian<uint32_t>(from_height)
    });

    send_request("address.fetch_history", data, std::move(on_error),
        std::bind(decode_fetch_history, _1, std::move(on_reply)));
}

void obelisk_codec::subscribe(error_handler on_error,
    empty_handler on_reply,
    const payment_address& address)
{
    binary_type prefix((short_hash_size * byte_bits), address.hash());

    // [ type:1 ] (0 = address prefix, 1 = stealth prefix)
    // [ prefix_bitsize:1 ]
    // [ prefix_blocks:...  ]
    auto data = build_data({
        to_byte(static_cast<uint8_t>(subscribe_type::address)),
        to_byte(static_cast<uint8_t>(prefix.size())),
        prefix.blocks()
    });

    send_request("address.subscribe", data, std::move(on_error),
        std::bind(decode_empty, _1, std::move(on_reply)));
}

void obelisk_codec::renew(error_handler on_error,
    empty_handler on_reply,
    const payment_address& address)
{
    binary_type prefix((short_hash_size * byte_bits), address.hash());

    // [ type:1 ] (0 = address prefix, 1 = stealth prefix)
    // [ prefix_bitsize:1 ]
    // [ prefix_blocks:...  ]
    auto data = build_data({
        to_byte(static_cast<uint8_t>(subscribe_type::address)),
        to_byte(static_cast<uint8_t>(prefix.size())),
        prefix.blocks()
    });

    send_request("address.renew", data, std::move(on_error),
        std::bind(decode_empty, _1, std::move(on_reply)));
}

void obelisk_codec::subscribe(error_handler on_error,
    empty_handler on_reply,
    subscribe_type discriminator,
    const binary_type& prefix)
{
    // should this be a throw or should there be a return type instead?
    if (prefix.size() > max_uint8)
    {
        on_error(std::make_error_code(std::errc::bad_address));
        return;
    }

    auto data = build_data({
        to_byte(static_cast<uint8_t>(discriminator)),
        to_byte(static_cast<uint8_t>(prefix.size())),
        prefix.blocks()
    });

    send_request("address.subscribe", data, std::move(on_error),
        std::bind(decode_empty, _1, std::move(on_reply)));
}

// See below for description of updates data format.
//enum class subscribe_type : uint8_t
//{
//    address = 0,
//    stealth = 1
//};
//
//void obelisk_codec::subscribe(error_handler on_error,
//    empty_handler on_reply,
//    const address_prefix& prefix)
//{
//    // BUGBUG: assertion is not good enough here.
//    BITCOIN_ASSERT(prefix.size() <= 255);
//
//    // [ type:1 ] (0 = address prefix, 1 = stealth prefix)
//    // [ prefix_bitsize:1 ]
//    // [ prefix_blocks:...  ]
//    auto data = build_data({
//        to_byte(static_cast<uint8_t>(subscribe_type::address)),
//        to_byte(prefix.size()),
//        prefix.blocks()
//    });
//
//    send_request("address.subscribe", data, std::move(on_error),
//        std::bind(decode_empty, _1, std::move(on_reply)));
//}

/**
See also libbitcoin-server repo subscribe_manager::post_updates() and
subscribe_manager::post_stealth_updates().

The address result is:

    Response command = "address.update"

    [ version:1 ]
    [ hash:20 ]
    [ height:4 ]
    [ block_hash:32 ]

    struct address_subscribe_result
    {
        payment_address address;
        uint32_t height;
        hash_digest block_hash;
    };

When the subscription type is stealth, then the result is:

    Response command = "address.stealth_update"

    [ 32 bit prefix:4 ]
    [ height:4 ]
    [ block_hash:32 ]

    // Currently not used.
    struct stealth_subscribe_result
    {
        typedef byte_array<4> stealth_prefix_bytes;
        // Protocol will send back 4 bytes of prefix.
        // See libbitcoin-server repo subscribe_manager::post_stealth_updates()
        stealth_prefix_bytes prefix;
        uint32_t height;
        hash_digest block_hash;
    };

Subscriptions expire after 10 minutes. Therefore messages with the command
"address.renew" should be sent periodically to the server. The format
is the same as for "address.subscribe, and the server will respond
with a 4 byte error code.
*/

void obelisk_codec::decode_empty(data_deserial& payload,
    empty_handler& handler)
{
    check_end(payload);
    handler();
}

void obelisk_codec::decode_fetch_history(data_deserial& payload,
    fetch_history_handler& handler)
{
    history_list history;
    while (payload.iterator() != payload.end())
    {
        history_row row;
        row.output.hash = payload.read_hash();
        row.output.index = payload.read_4_bytes();
        row.output_height = payload.read_4_bytes();
        row.value = payload.read_8_bytes();
        row.spend.hash = payload.read_hash();
        row.spend.index = payload.read_4_bytes();
        row.spend_height = payload.read_4_bytes();
        history.push_back(row);
    }
    handler(history);
}

void obelisk_codec::decode_fetch_transaction(data_deserial& payload,
    fetch_transaction_handler& handler)
{
    transaction_type tx;
    satoshi_load(payload.iterator(), payload.end(), tx);
    payload.set_iterator(payload.iterator() + satoshi_raw_size(tx));
    check_end(payload);
    handler(tx);
}

void obelisk_codec::decode_fetch_last_height(data_deserial& payload,
    fetch_last_height_handler& handler)
{
    uint32_t last_height = payload.read_4_bytes();
    check_end(payload);
    handler(last_height);
}

void obelisk_codec::decode_fetch_block_header(data_deserial& payload,
    fetch_block_header_handler& handler)
{
    block_header_type header;
    satoshi_load(payload.iterator(), payload.end(), header);
    payload.set_iterator(payload.iterator() + satoshi_raw_size(header));
    check_end(payload);
    handler(header);
}

void obelisk_codec::decode_fetch_transaction_index(data_deserial& payload,
    fetch_transaction_index_handler& handler)
{
    uint32_t block_height = payload.read_4_bytes();
    uint32_t index = payload.read_4_bytes();
    check_end(payload);
    handler(block_height, index);
}

void obelisk_codec::decode_fetch_stealth(data_deserial& payload,
    fetch_stealth_handler& handler)
{
    stealth_list results;
    while (payload.iterator() != payload.end())
    {
        stealth_row row;

        // presume first byte based on convention
        row.ephemkey = payload.read_data(32);
        row.ephemkey.insert(row.ephemkey.begin(), 0x02);

        // presume address_version
        uint8_t address_version = payment_address::pubkey_version;
        const short_hash address_hash = payload.read_short_hash();
        row.address.set(address_version, reverse(address_hash));

        row.transaction_hash = payload.read_hash();

        results.push_back(row);
    }

    handler(results);
}

void obelisk_codec::decode_validate(data_deserial& payload,
    validate_handler& handler)
{
    index_list unconfirmed;
    while (payload.iterator() != payload.end())
    {
        uint32_t unconfirm_index = payload.read_4_bytes();
        unconfirmed.push_back(unconfirm_index);
    }
    handler(unconfirmed);
}

} // namespace client
} // namespace libbitcoin
