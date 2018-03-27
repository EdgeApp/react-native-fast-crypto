/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "JsonObject.hpp"

namespace abcd {

JsonObject::JsonObject():
    JsonPtr(json_object())
{
}

JsonObject::JsonObject(JsonPtr &&move):
    JsonPtr(std::move(move))
{
}

JsonObject::JsonObject(const JsonPtr &copy):
    JsonPtr(copy)
{
}

Status
JsonObject::ok()
{
    if (!json_is_object(root_))
        return ABC_ERROR(ABC_CC_JSONError, "Not a JSON object.");
    return Status();
}

#define IMPLEMENT_HAS(test) \
    json_t *value = json_object_get(root_, key); \
    if (!value || !(test)) \
        return ABC_ERROR(ABC_CC_JSONError, "Bad JSON value for " + std::string(key)); \
    return Status();

Status
JsonObject::hasString(const char *key) const
{
    IMPLEMENT_HAS(json_is_string(value))
}

Status
JsonObject::hasNumber(const char *key) const
{
    IMPLEMENT_HAS(json_is_number(value))
}

Status
JsonObject::hasBoolean(const char *key) const
{
    IMPLEMENT_HAS(json_is_boolean(value))
}

Status
JsonObject::hasInteger(const char *key) const
{
    IMPLEMENT_HAS(json_is_integer(value))
}

#define IMPLEMENT_GET(type) \
    json_t *value = json_object_get(root_, key); \
    if (!value || !json_is_##type(value)) \
        return fallback; \
    return json_##type##_value(value);

const char *
JsonObject::getString(const char *key, const char *fallback) const
{
    IMPLEMENT_GET(string)
}

double
JsonObject::getNumber(const char *key, double fallback) const
{
    IMPLEMENT_GET(number)
}

bool
JsonObject::getBoolean(const char *key, bool fallback) const
{
    IMPLEMENT_GET(boolean)
}

json_int_t
JsonObject::getInteger(const char *key, json_int_t fallback) const
{
    IMPLEMENT_GET(integer)
}

JsonPtr
JsonObject::getValue(const char *key) const
{
    return json_incref(json_object_get(root_, key));
}

Status
JsonObject::set(const char *key, JsonPtr value)
{
    return setRaw(key, json_incref(value.get()));
}

Status
JsonObject::set(const char *key, const char *value)
{
    return setRaw(key, json_string(value));
}

Status
JsonObject::set(const char *key, const std::string &value)
{
    return setRaw(key, json_string(value.c_str()));
}

Status
JsonObject::set(const char *key, double value)
{
    return setRaw(key, json_real(value));
}

Status
JsonObject::set(const char *key, bool value)
{
    return setRaw(key, json_boolean(value));
}

Status
JsonObject::set(const char *key, json_int_t value)
{
    return setRaw(key, json_integer(value));
}

Status
JsonObject::pick(JsonObject in, const std::vector<std::string> &keys)
{
    for (const auto key: keys)
    {
        const auto value = in.getValue(key.c_str());
        if (value)
            ABC_CHECK(set(key.c_str(), value));
    }
    return Status();
}

Status
JsonObject::setRaw(const char *key, json_t *value)
{
    if (json_object_set_new(root_, key, value) < 0)
        return ABC_ERROR(ABC_CC_JSONError, "Cannot set " + std::string(key));
    return Status();
}

} // namespace abcd
