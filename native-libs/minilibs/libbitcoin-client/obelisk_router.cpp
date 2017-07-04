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
#include "obelisk_router.hpp"

namespace libbitcoin {
namespace client {

using std::placeholders::_1;

obelisk_router::obelisk_router(std::shared_ptr<message_stream> out)
  : last_request_id_(0),
    timeout_(std::chrono::seconds(2)), retries_(0),
    on_unknown_(on_unknown_nop), on_update_(on_update_nop),
    on_stealth_update_(on_stealth_update_nop), out_(out)
{
}

obelisk_router::~obelisk_router()
{
    for (const auto &request: pending_requests_)
    {
        const auto ec = std::make_error_code(std::errc::operation_canceled);
        request.second.on_error(ec);
    }
}

void obelisk_router::set_on_update(update_handler on_update)
{
    on_update_ = std::move(on_update);
}

void obelisk_router::set_on_stealth_update(
    stealth_update_handler on_update)
{
    on_stealth_update_ = std::move(on_update);
}

void obelisk_router::set_on_unknown(unknown_handler on_unknown)
{
    on_unknown_ = std::move(on_unknown);
}

void obelisk_router::set_retries(uint8_t retries)
{
    retries_ = retries;
}

void obelisk_router::set_timeout(period_ms timeout)
{
    timeout_ = timeout;
}

uint64_t obelisk_router::outstanding_call_count() const
{
    return pending_requests_.size();
}

void obelisk_router::write(const data_stack& data)
{
    if (data.size() == 3)
    {
        bool success = true;
        obelisk_message message;

        auto it = data.begin();

        if (success)
        {
            // read command
            message.command = std::string((*it).begin(), (*it).end());
            it++;
        }

        if (success)
        {
            // read id
            if ((*it).size() == sizeof(uint32_t))
            {
                message.id = from_little_endian_unsafe<uint32_t>(
                    (*it).begin());
            }
            else
            {
                success = false;
            }

            it++;
        }

        if (success)
        {
            // read payload
            message.payload = (*it);
            it++;
        }

        receive(message);
    }
}

period_ms obelisk_router::wakeup()
{
    period_ms next_wakeup(0);
    auto now = std::chrono::steady_clock::now();

    auto i = pending_requests_.begin();
    while (i != pending_requests_.end())
    {
        auto request = i++;
        auto elapsed = std::chrono::duration_cast<period_ms>(
            now - request->second.last_action);
        if (timeout_ <= elapsed)
        {
            if (request->second.retries < retries_)
            {
                // Resend:
                ++request->second.retries;
                request->second.last_action = now;
                next_wakeup = min_sleep(next_wakeup, timeout_);
                send(request->second.message);
            }
            else
            {
                // Cancel:
                auto ec = std::make_error_code(std::errc::timed_out);
                request->second.on_error(ec);
                pending_requests_.erase(request);
            }
        }
        else
            next_wakeup = min_sleep(next_wakeup, timeout_ - elapsed);
    }
    return next_wakeup;
}

void obelisk_router::send_request(const std::string& command,
    const data_chunk& payload,
    error_handler on_error, decoder on_reply)
{
    uint32_t id = ++last_request_id_;
    pending_request& request = pending_requests_[id];
    request.message = obelisk_message{command, id, payload};
    request.on_error = std::move(on_error);
    request.on_reply = std::move(on_reply);
    request.retries = 0;
    request.last_action = std::chrono::steady_clock::now();
    send(request.message);
}

void obelisk_router::send(const obelisk_message& message)
{
    if (out_)
    {
        data_stack data;
        data.push_back(to_data_chunk(message.command));
        data.push_back(to_data_chunk(to_little_endian(message.id)));
        data.push_back(message.payload);
        out_->write(data);
    }
}

void obelisk_router::receive(const obelisk_message& message)
{
    if ("address.update" == message.command)
    {
        decode_update(message);
        return;
    }

    if ("address.stealth_update" == message.command)
    {
        decode_stealth_update(message);
        return;
    }

    auto i = pending_requests_.find(message.id);
    if (i == pending_requests_.end())
    {
        on_unknown_(message.command);
        return;
    }
    decode_reply(message, i->second.on_error, i->second.on_reply);
    pending_requests_.erase(i);
}

void obelisk_router::decode_update(const obelisk_message& message)
{
    data_deserial deserial = make_deserializer(
        message.payload.begin(), message.payload.end());
    try
    {
        // This message does not have an error_code at the beginning.
        uint8_t version_byte = deserial.read_byte();
        short_hash addr_hash = deserial.read_short_hash();
        payment_address address(version_byte, addr_hash);
        uint32_t height = deserial.read_4_bytes();
        hash_digest blk_hash = deserial.read_hash();
        transaction_type tx;
        satoshi_load(deserial.iterator(), deserial.end(), tx);
        deserial.set_iterator(deserial.iterator() + satoshi_raw_size(tx));
        check_end(deserial);
        on_update_(address, height, blk_hash, tx);
    }
    catch (end_of_stream)
    {
        on_unknown_(message.command);
    }
}

void obelisk_router::decode_stealth_update(const obelisk_message& message)
{
    data_deserial deserial = make_deserializer(
        message.payload.begin(), message.payload.end());
    try
    {
        // This message does not have an error_code at the beginning.
        data_chunk raw_prefix;
        raw_prefix.push_back(deserial.read_byte());
        raw_prefix.push_back(deserial.read_byte());
        raw_prefix.push_back(deserial.read_byte());
        raw_prefix.push_back(deserial.read_byte());
        binary_type prefix(32, raw_prefix);

        uint32_t height = deserial.read_4_bytes();
        hash_digest blk_hash = deserial.read_hash();
        transaction_type tx;
        satoshi_load(deserial.iterator(), deserial.end(), tx);
        deserial.set_iterator(deserial.iterator() + satoshi_raw_size(tx));
        check_end(deserial);
        on_stealth_update_(prefix, height, blk_hash, tx);
    }
    catch (end_of_stream)
    {
        on_unknown_(message.command);
    }
}

void obelisk_router::decode_reply(const obelisk_message& message,
    error_handler& on_error, decoder& on_reply)
{
    std::error_code ec;
    data_deserial deserial = make_deserializer(
        message.payload.begin(), message.payload.end());
    try
    {
        uint32_t val = deserial.read_4_bytes();
        if (val)
            ec = static_cast<error::error_code_t>(val);
        else
            on_reply(deserial);
    }
    catch (end_of_stream)
    {
        ec = std::make_error_code(std::errc::bad_message);
    }
    if (ec)
        on_error(ec);
}

void obelisk_router::check_end(data_deserial& payload)
{
    if (payload.iterator() != payload.end())
        throw end_of_stream();
}

void obelisk_router::on_unknown_nop(const std::string&)
{
}

void obelisk_router::on_update_nop(const payment_address&,
    size_t, const hash_digest&, const transaction_type&)
{
}

void obelisk_router::on_stealth_update_nop(const binary_type&,
    size_t, const hash_digest&, const transaction_type&)
{
}

} // namespace client
} // namespace libbitcoin
