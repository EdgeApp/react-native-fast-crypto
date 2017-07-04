/**
 * @file
 * Core libgit2-based file-syncing algorithm.
 *
 * See LICENSE for copy, modification, and use permissions
 *
 * @author See AUTHORS
 * @version 1.0
 */

#ifndef SYNC_H
#define SYNC_H

#include <git2.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Fetches the contents of the server into the "incoming" branch.
 */
int sync_fetch(git_repository *repo,
               const char *server);

/**
 * Updates the master branch with the latest changes, including local
 * changes and changes from the remote repository.
 * @param files_changed set to 1 if the function has changed the workdir.
 * @param need_push set to 1 if the master branch has changes not on the
 * server.
 */
int sync_master(git_repository *repo,
                int *files_changed,
                int *need_push);

/**
 * Pushes the master branch to the server.
 */
int sync_push(git_repository *repo,
              const char *server);

#ifdef __cplusplus
}
#endif

#endif
