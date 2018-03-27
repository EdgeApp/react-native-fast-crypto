/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "Data.hpp"

namespace abcd {

std::string
toString(DataSlice slice)
{
    // Due to a bug, lots of AirBitz encrypted blobs end with a null byte.
    // Get rid of those:
    auto size = slice.size();
    if (0 < size && !slice.data()[size - 1])
        size--;

    return std::string(reinterpret_cast<const char *>(slice.data()), size);
}

DataChunk
buildData(std::initializer_list<DataSlice> slices)
{
    size_t size = 0;
    for (auto slice: slices)
        size += slice.size();

    DataChunk out;
    out.reserve(size);
    for (auto slice: slices)
        out.insert(out.end(), slice.begin(), slice.end());
    return out;
}

} // namespace abcd
