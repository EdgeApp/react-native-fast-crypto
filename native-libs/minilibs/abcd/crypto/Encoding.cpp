/*
 * Copyright (c) 2015, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "Encoding.hpp"
#include <bitcoin/bitcoin.hpp>
#include <algorithm>

namespace abcd {

/**
 * Encodes data in an arbitrary power-of-2 base.
 * @param Bytes number of bytes per chunk of characters.
 * @param Chars number of characters per chunk.
 */
template<unsigned Bytes, unsigned Chars> std::string
chunkEncode(DataSlice data, const char *alphabet)
{
    std::string out;
    auto chunks = (data.size() + Bytes - 1) / Bytes; // Rounding up
    out.reserve(Chars * chunks);

    constexpr unsigned shift = 8 * Bytes / Chars; // Bits per character
    uint16_t buffer = 0; // Bits waiting to be written out, MSB first
    signed bits = 0; // Number of bits currently in the buffer
    auto i = data.begin();
    while (i != data.end() || 0 < bits)
    {
        // Reload the buffer if we need more bits:
        if (i != data.end() && static_cast<unsigned>(bits) < shift)
        {
            buffer |= *i++ << (8 - bits);
            bits += 8;
        }

        // Write out the most-significant bits in the buffer:
        out += alphabet[buffer >> (16 - shift)];
        buffer <<= shift;
        bits -= shift;
    }

    // Pad the final string to a multiple of the chunk size:
    out.append(-out.size() % Chars, '=');
    return out;
}

/**
 * Decodes data from an arbitrary power-of-2 base.
 * @param Bytes number of bytes per chunk of characters.
 * @param Chars number of characters per chunk.
 * @param Decode function for converting characters to their values.
 * A negative value indicates an invalid character.
 */
template<unsigned Bytes, unsigned Chars, int Decode(char c)>
Status
chunkDecode(DataChunk &result, const std::string &in)
{
    // The string must be a multiple of the chunk size:
    if (in.size() % Chars)
        return ABC_ERROR(ABC_CC_ParseError, "Bad encoding");

    DataChunk out;
    out.reserve(Bytes * (in.size() / Chars));

    constexpr unsigned shift = 8 * Bytes / Chars; // Bits per character
    uint16_t buffer = 0; // Bits waiting to be written out, MSB first
    unsigned bits = 0; // Number of bits currently in the buffer
    auto i = in.begin();
    while (i != in.end())
    {
        // Read one character from the string:
        const auto value = Decode(*i);
        if (value < 0)
            break;
        ++i;

        // Append the bits to the buffer:
        buffer |= value << (16 - bits - shift);
        bits += shift;

        // Write out some bits if the buffer has a byte's worth:
        if (8 <= bits)
        {
            out.push_back(buffer >> 8);
            buffer <<= 8;
            bits -= 8;
        }
    }

    // Any extra characters must be '=':
    const auto isPadding = [](char c) { return '=' == c; };
    if (!std::all_of(i, in.end(), isPadding))
        return ABC_ERROR(ABC_CC_ParseError, "Bad encoding");

    // There cannot be extra padding:
    if (Chars <= in.end() - i || shift <= bits)
        return ABC_ERROR(ABC_CC_ParseError, "Bad encoding");

    // Any extra bits must be 0 (but rfc4648 decoders can be liberal here):
//    if (buffer != 0)
//        return false;

    result = std::move(out);
    return Status();
}

static int
base16Decode(char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    if ('A' <= c && c <= 'F')
        return 10 + c - 'A';
    if ('a' <= c && c <= 'f')
        return 10 + c - 'a';
    return -1;
}

static int
base32Decode(char c)
{
    if ('A' <= c && c <= 'Z')
        return c - 'A';
    if ('2' <= c && c <= '7')
        return 26 + c - '2';
    return -1;
}

static int
base64Decode(char c)
{
    if ('A' <= c && c <= 'Z')
        return c - 'A';
    if ('a' <= c && c <= 'z')
        return 26 + c - 'a';
    if ('0' <= c && c <= '9')
        return 52 + c - '0';
    if ('+' == c)
        return 62;
    if ('/' == c)
        return 63;
    return -1;
}

std::string
base16Encode(DataSlice data)
{
    return chunkEncode<1, 2>(data, "0123456789abcdef");
}

Status
base16Decode(DataChunk &result, const std::string &in)
{
    return chunkDecode<1, 2, base16Decode>(result, in);
}

std::string
base32Encode(DataSlice data)
{
    return chunkEncode<5, 8>(data, "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567");
}

Status
base32Decode(DataChunk &result, const std::string &in)
{
    return chunkDecode<5, 8, base32Decode>(result, in);
}

std::string
base58Encode(DataSlice data)
{
    return bc::encode_base58(data);
}

Status
base58Decode(DataChunk &result, const std::string &in)
{
    if (!bc::decode_base58(result, in))
        return ABC_ERROR(ABC_CC_ParseError, "Bad encoding");

    return Status();
}

std::string
base64Encode(DataSlice data)
{
    return chunkEncode<3, 4>(data,
                             "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
}

Status
base64Decode(DataChunk &result, const std::string &in)
{
    return chunkDecode<3, 4, base64Decode>(result, in);
}

} // namespace abcd
