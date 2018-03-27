/*
 * Copyright (c) 2015, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#ifndef ABCD_JSON_JSON_ARRAY_HPP
#define ABCD_JSON_JSON_ARRAY_HPP

#include "JsonPtr.hpp"

namespace abcd {

/**
 * A JsonPtr with an array as it's root element.
 */
class JsonArray:
    public JsonPtr
{
public:
    JsonArray();
    JsonArray(JsonPtr &&move);
    JsonArray(const JsonPtr &copy);

    /**
     * Returns an error if this is not actually pointing to a JSON array.
     */
    Status
    ok();

    /**
     * Returns the number of values in the array.
     */
    size_t size() { return json_array_size(root_); }

    /**
     * Retrieves a value from the array.
     * Returns a null value if something goes wrong.
     */
    JsonPtr
    operator[](size_t i);

    /**
     * Appends a value to the end of an array.
     */
    Status
    append(JsonPtr value);
};

} // namespace abcd

#endif
