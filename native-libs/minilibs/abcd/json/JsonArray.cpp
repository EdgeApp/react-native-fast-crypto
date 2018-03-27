/*
 * Copyright (c) 2015, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "JsonArray.hpp"

namespace abcd {

JsonArray::JsonArray():
    JsonPtr(json_array())
{
}

JsonArray::JsonArray(JsonPtr &&move):
    JsonPtr(std::move(move))
{
}

JsonArray::JsonArray(const JsonPtr &copy):
    JsonPtr(copy)
{
}

Status
JsonArray::ok()
{
    if (!json_is_array(root_))
        return ABC_ERROR(ABC_CC_JSONError, "Not a JSON array.");
    return Status();
}

JsonPtr
JsonArray::operator[](size_t i)
{
    return json_incref(json_array_get(root_, i));
}

Status
JsonArray::append(JsonPtr value)
{
    if (json_array_append(root_, value.get()) < 0)
        return ABC_ERROR(ABC_CC_JSONError, "Cannot append to array");
    return Status();
}

} // namespace abcd
