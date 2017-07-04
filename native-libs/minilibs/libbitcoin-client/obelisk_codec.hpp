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
#ifndef LIBBITCOIN_CLIENT_OBELISK_OBELISK_CODEC_HPP
#define LIBBITCOIN_CLIENT_OBELISK_OBELISK_CODEC_HPP

#include <functional>
#include "obelisk_router.hpp"
#include "obelisk_types.hpp"

namespace libbitcoin {
namespace client {

/**
 * Decodes and encodes messages in the obelisk protocol.
 * This class is a pure codec; it does not talk directly to zeromq.
 */
class obelisk_codec
  : public obelisk_router
{
public:
    /**
     * Constructor.
     * @param out a stream to receive outgoing messages created by the codec.
     * @param on_update function to handle subscription update messages.
     * @param on_unknown function to handle malformed incoming messages.
     * @param timeout the call timeout. Defaults to 2 seconds.
     * @param retries the number of retries to attempt.
     */
    obelisk_codec(
        std::shared_ptr<message_stream> out,
        update_handler on_update=on_update_nop,
        unknown_handler on_unknown=on_unknown_nop,
        period_ms timeout=std::chrono::seconds(2),
        uint8_t retries=0);

    virtual ~obelisk_codec();

    // Message reply handlers:
    typedef std::function<void (const history_list&)>
        fetch_history_handler;
    typedef std::function<void (const transaction_type&)>
        fetch_transaction_handler;
    typedef std::function<void (size_t)>
        fetch_last_height_handler;
    typedef std::function<void (const block_header_type&)>
        fetch_block_header_handler;
    typedef std::function<void (size_t block_height, size_t index)>
        fetch_transaction_index_handler;
    typedef std::function<void (const stealth_list&)>
        fetch_stealth_handler;
    typedef std::function<void (const index_list& unconfirmed)>
        validate_handler;
    typedef std::function<void ()> empty_handler;

    // Outgoing messages:
    void fetch_history(error_handler on_error,
        fetch_history_handler on_reply,
        const payment_address& address, uint32_t from_height=0);
    void fetch_transaction(error_handler on_error,
        fetch_transaction_handler on_reply,
        const hash_digest& tx_hash);
    void fetch_last_height(error_handler on_error,
        fetch_last_height_handler on_reply);
    void fetch_block_header(error_handler on_error,
        fetch_block_header_handler on_reply,
        uint32_t height);
    void fetch_block_header(error_handler on_error,
        fetch_block_header_handler on_reply,
        const hash_digest& blk_hash);
    void fetch_transaction_index(error_handler on_error,
        fetch_transaction_index_handler on_reply,
        const hash_digest& tx_hash);
    void fetch_stealth(error_handler on_error,
        fetch_stealth_handler on_reply,
        const bc::binary_type& prefix, uint32_t from_height=0);
    void validate(error_handler on_error,
        validate_handler on_reply,
        const transaction_type& tx);
    void fetch_unconfirmed_transaction(error_handler on_error,
        fetch_transaction_handler on_reply,
        const hash_digest& tx_hash);
    void broadcast_transaction(error_handler on_error,
        empty_handler on_reply,
        const transaction_type& tx);
    void address_fetch_history(error_handler on_error,
        fetch_history_handler on_reply,
        const payment_address& address, uint32_t from_height=0);
    void subscribe(error_handler on_error,
        empty_handler on_reply,
        const bc::payment_address& address);
    void renew(error_handler on_error,
        empty_handler on_reply,
        const bc::payment_address& address);
    void subscribe(error_handler on_error,
        empty_handler on_reply,
        subscribe_type discriminator,
        const bc::binary_type& prefix);

private:
    static void decode_empty(data_deserial& payload,
        empty_handler& handler);
    static void decode_fetch_history(data_deserial& payload,
        fetch_history_handler& handler);
    static void decode_fetch_transaction(data_deserial& payload,
        fetch_transaction_handler& handler);
    static void decode_fetch_last_height(data_deserial& payload,
        fetch_last_height_handler& handler);
    static void decode_fetch_block_header(data_deserial& payload,
        fetch_block_header_handler& handler);
    static void decode_fetch_transaction_index(data_deserial& payload,
        fetch_transaction_index_handler& handler);
    static void decode_fetch_stealth(data_deserial& payload,
        fetch_stealth_handler& handler);
    static void decode_validate(data_deserial& payload,
        validate_handler& handler);
};

} // namespace client
} // namespace libbitcoin

#endif

