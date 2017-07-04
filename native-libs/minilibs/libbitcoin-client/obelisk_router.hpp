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
#ifndef LIBBITCOIN_CLIENT_OBELISK_OBELISK_ROUTER_HPP
#define LIBBITCOIN_CLIENT_OBELISK_OBELISK_ROUTER_HPP

#include <functional>
#include <map>
#include "message_stream.hpp"
#include "sleeper.hpp"

namespace libbitcoin {
namespace client {

/**
 * Matches replies with outgoing messages, accounting for timeouts and
 * retries.
 * This class is a pure codec; it does not talk directly to zeromq.
 */
class obelisk_router
  : public message_stream, public sleeper
{
public:
    /**
     * Constructor.
     * @param out a stream to receive outgoing messages created by the codec.
     */
    obelisk_router(std::shared_ptr<message_stream> out);

    virtual ~obelisk_router();

    // Loose message handlers:
    typedef std::function<void (const std::error_code&)> error_handler;
    typedef std::function<void (const std::string& command)> unknown_handler;
    typedef std::function<void (const payment_address& address, size_t height,
        const hash_digest& blk_hash, const transaction_type&)> update_handler;
    typedef std::function<void (const binary_type& prefix, size_t height,
        const hash_digest& blk_hash, const transaction_type& tx)> stealth_update_handler;

    static void on_unknown_nop(const std::string&);
    static void on_update_nop(const payment_address&,
        size_t, const hash_digest&, const transaction_type&);
    static void on_stealth_update_nop(const binary_type&, size_t,
            const hash_digest&, const transaction_type&);

    virtual void set_on_update(update_handler on_update);
    virtual void set_on_stealth_update(stealth_update_handler on_update);
    virtual void set_on_unknown(unknown_handler on_unknown);
    virtual void set_retries(uint8_t retries);
    virtual void set_timeout(period_ms timeout);

    uint64_t outstanding_call_count() const;

    // message-stream interface:
    virtual void write(const data_stack& data) override;

    // sleeper interface:
    virtual period_ms wakeup() override;

protected:
    typedef deserializer<data_chunk::const_iterator, true> data_deserial;

    /**
     * Decodes a message and calls the appropriate callback.
     * By the time this is called, the error code has already been read
     * out of the payload and checked.
     * If there is something wrong with the payload, this function should
     * throw a end_of_stream exception.
     */
    typedef std::function<void (data_deserial& payload)> decoder;

    /**
     * Sends an outgoing request, and adds the handlers to the pending
     * request table.
     */
    void send_request(const std::string& command,
        const data_chunk& payload,
        error_handler on_error, decoder on_reply);

    struct obelisk_message
    {
        std::string command;
        uint32_t id;
        data_chunk payload;
    };
    void send(const obelisk_message& message);
    void receive(const obelisk_message& message);
    void decode_update(const obelisk_message& message);
    void decode_stealth_update(const obelisk_message& message);
    void decode_reply(const obelisk_message& message,
        error_handler& on_error, decoder& on_reply);

    /**
     * Verifies that the deserializer has reached the end of the payload,
     * and throws end_of_stream if not.
     */
    static void check_end(data_deserial& payload);

    // Request management:
    uint32_t last_request_id_;
    struct pending_request
    {
        obelisk_message message;
        error_handler on_error;
        decoder on_reply;
        unsigned retries;
        std::chrono::steady_clock::time_point last_action;
    };
    std::map<uint32_t, pending_request> pending_requests_;

    // Timeout parameters:
    period_ms timeout_;
    uint8_t retries_;

    // Loose-message event handlers:
    unknown_handler on_unknown_;
    update_handler on_update_;
    stealth_update_handler on_stealth_update_;

    // Outgoing message stream:
    std::shared_ptr<message_stream> out_;
};

} // namespace client
} // namespace libbitcoin

#endif

