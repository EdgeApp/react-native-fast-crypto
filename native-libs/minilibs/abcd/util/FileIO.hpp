/*
 * Copyright (c) 2015, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */
/**
 * @file
 * Filesystem access functions
 */

#ifndef ABCD_UTIL_FILE_IO_HPP
#define ABCD_UTIL_FILE_IO_HPP

#include "Data.hpp"
#include "Status.hpp"
#include <time.h>

namespace abcd {

/**
 * Puts a slash on the end of a filename (if necessary).
 */
std::string
fileSlashify(const std::string &path);

/**
 * Returns true if a filename (without the directory part) ends with ".json".
 */
bool
fileIsJson(const std::string &name);

/**
 * Ensures that a directory exists, creating it if not.
 */
Status
fileEnsureDir(const std::string &dir);

/**
 * Returns true if the path exists.
 */
bool
fileExists(const std::string &path);

/**
 * Reads a file from disk.
 */
Status
fileLoad(DataChunk &result, const std::string &path);

/**
 * Writes a file to disk.
 */
Status
fileSave(DataSlice data, const std::string &path);

/**
 * Deletes a file recursively.
 */
Status
fileDelete(const std::string &path);

/**
 * Determines a file's last-modification time.
 */
Status
fileTime(time_t &result, const std::string &path);

} // namespace abcd

#endif
