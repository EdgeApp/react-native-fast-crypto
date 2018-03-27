/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */
/**
 * @file
 * Helper classes for dealing with raw data.
 */

#ifndef ABCD_UTIL_DATA_HPP
#define ABCD_UTIL_DATA_HPP

#include <stdint.h>
#include <array>
#include <string>
#include <vector>

namespace abcd {

/**
 * A reference to a block of raw data.
 */
class DataSlice
{
public:
    DataSlice():
        begin_(nullptr), end_(nullptr)
    {}

    DataSlice(const uint8_t *begin, const uint8_t *end):
        begin_(begin), end_(end)
    {}

    template<typename Container>
    DataSlice(const Container &container):
        begin_(container.data()), end_(container.data() + container.size())
    {}

    DataSlice(const std::string &s):
        begin_(reinterpret_cast<const uint8_t *>(s.data())),
        end_(reinterpret_cast<const uint8_t *>(s.data()) + s.size())
    {}

    // STL-sytle accessors:
    bool empty()            const { return end_ == begin_; }
    std::size_t size()      const { return end_ - begin_; }
    const uint8_t *data()   const { return begin_; }
    const uint8_t *begin()  const { return begin_; }
    const uint8_t *end()    const { return end_; }

private:
    const uint8_t *begin_;
    const uint8_t *end_;
};

/**
 * Casts a data slice to a string.
 */
std::string
toString(DataSlice slice);

/**
 * A block of data with its size fixed at compile time.
 */
template<size_t Size> using DataArray = std::array<uint8_t, Size>;

/**
 * A block of data with a run-time variable size.
 */
typedef std::vector<uint8_t> DataChunk;

/**
 * Concatenates several data slices into a single buffer.
 */
DataChunk
buildData(std::initializer_list<DataSlice> slices);

} // namespace abcd

#endif
