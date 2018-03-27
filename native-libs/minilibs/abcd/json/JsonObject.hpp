
/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#ifndef ABCD_JSON_JSON_OBJECT_HPP
#define ABCD_JSON_JSON_OBJECT_HPP

#include "JsonPtr.hpp"

namespace abcd {

/**
 * A JsonPtr with an object (key-value pair) as it's root element.
 * This allows all sorts of member lookups.
 */
class JsonObject:
    public JsonPtr
{
public:
    JsonObject();
    JsonObject(JsonPtr &&move);
    JsonObject(const JsonPtr &copy);

    /**
     * Returns an error if this is not actually pointing to a JSON object.
     */
    Status
    ok();

    // Type helpers:
    Status hasString (const char *key) const;
    Status hasNumber (const char *key) const;
    Status hasBoolean(const char *key) const;
    Status hasInteger(const char *key) const;

    // Read helpers:
    const char *getString (const char *key, const char *fallback) const;
    double      getNumber (const char *key, double fallback) const;
    bool        getBoolean(const char *key, bool fallback) const;
    json_int_t  getInteger(const char *key, json_int_t fallback) const;
    JsonPtr     getValue  (const char *key) const;

    // Set helpers:
    Status set(const char *key, JsonPtr value);
    Status set(const char *key, const char *value);
    Status set(const char *key, const std::string &value);
    Status set(const char *key, double value);
    Status set(const char *key, bool value);
    Status set(const char *key, json_int_t value);

    /**
     * Copies the selected fields from the provided object into this object.
     */
    Status
    pick(JsonObject in, const std::vector<std::string> &keys);

private:
    /**
     * Writes a key-value pair to the object.
     * Takes ownership of the passed-in value.
     */
    Status
    setRaw(const char *key, json_t *value);
};

// Helper macros for implementing JsonObject child classes:

#define ABC_JSON_VALUE(name, key, Type) \
    Type name() const                           { return Type(json_incref(json_object_get(root_, key))); } \
    abcd::Status name##Set(const JsonPtr &value){ return set(key, value); }

#define ABC_JSON_STRING(name, key, fallback) \
    const char *name() const                    { return getString(key, fallback); } \
    abcd::Status name##Ok() const               { return hasString(key); } \
    abcd::Status name##Set(const char *value)   { return set(key, value); } \
    abcd::Status name##Set(const std::string &value) { return set(key, value); }

#define ABC_JSON_NUMBER(name, key, fallback) \
    double name() const                         { return getNumber(key, fallback); } \
    abcd::Status name##Ok() const               { return hasNumber(key); } \
    abcd::Status name##Set(double value)        { return set(key, value); }

#define ABC_JSON_BOOLEAN(name, key, fallback) \
    bool name() const                           { return getBoolean(key, fallback); } \
    abcd::Status name##Ok() const               { return hasBoolean(key); } \
    abcd::Status name##Set(bool value)          { return set(key, value); }

#define ABC_JSON_INTEGER(name, key, fallback) \
    json_int_t name() const                     { return getInteger(key, fallback); } \
    abcd::Status name##Ok() const               { return hasInteger(key); } \
    abcd::Status name##Set(json_int_t value)    { return set(key, value); }

} // namespace abcd

#endif
