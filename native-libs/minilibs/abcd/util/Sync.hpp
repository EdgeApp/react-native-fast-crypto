/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */
/**
 * @file
 * Wrappers around the AirBitz git-sync library.
 */

#ifndef ABC_Sync_h
#define ABC_Sync_h

#include "Status.hpp"

#define SYNC_KEY_LENGTH 20

namespace abcd {

/**
 * Initializes the underlying git library.
 * Should be called at program start.
 */
Status
syncInit(const char *szCaCertPath);

/**
 * Shuts down the underlying git library.
 * Should be called when the program exits.
 */
void
syncTerminate();

/**
 * Prepares a directory for syncing.
 * This will create the directory if it does not exist already.
 * Has no effect if the repo has already been created.
 */
Status
syncMakeRepo(const std::string &syncDir);

/**
 * Ensure that a sync directory exists.
 * Has no effect if the repo has already been created.
 * The temporary directory allows the initial clone to happen atomically.
 */
Status
syncEnsureRepo(const std::string &syncDir, const std::string &tempDir,
               const std::string &syncKey);

/**
 * Synchronizes the directory with the server.
 * New files in the folder will go up to the server,
 * and new files on the server will come down to the directory.
 * If there is a conflict, the server's file will win.
 * @param dirty set to true if the sync has modified the filesystem,
 * or false otherwise.
 */
Status
syncRepo(const std::string &syncDir, const std::string &syncKey, bool &dirty);

} // namespace abcd

#endif
