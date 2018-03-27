/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "Sync.hpp"
#include "AutoFree.hpp"
#include "Debug.hpp"
#include "FileIO.hpp"
#include "../Context.hpp"
#include "../General.hpp"
#include "../../minilibs/git-sync/sync.h"
#include <assert.h>
#include <stdlib.h>
#include <mutex>

namespace abcd {

std::recursive_mutex gSyncMutex;
static bool gbInitialized = false;
static int syncServerIndex;
static std::string syncServerName;
static std::vector<std::string> syncServers;

typedef std::lock_guard<std::recursive_mutex> AutoSyncLock;

#define ABC_CHECK_GIT(f) \
    do { \
        int ABC_e = (f); \
        if (ABC_e < 0) \
            return ABC_ERROR(ABC_CC_SysError, syncGitError(ABC_e)); \
    } while (false)

/**
 * Formats the error information produced by libgit2.
 */
static std::string
syncGitError(int e)
{
    std::string out = "libgit2 returned " + std::to_string(e);
    const git_error *error = giterr_last();
    if (error && error->message)
    {
        out += ": ";
        out += error->message;
    }
    return out;
}

/**
 * Builds a URL for the current git server.
 */
static Status
syncUrl(std::string &result, const std::string &syncKey, bool rotate=false)
{
    if (syncServers.size() == 0)
    {
        syncServers = generalSyncServers();
    }
    if (rotate || syncServerName.empty())
    {
        syncServerIndex++;
        syncServerIndex %= syncServers.size();
        syncServerName = fileSlashify(syncServers[syncServerIndex]);
    }

    result = syncServerName + syncKey;
    return Status();
}

Status
syncInit(const char *szCaCertPath)
{
    AutoSyncLock lock(gSyncMutex);

    if (gbInitialized)
        return ABC_ERROR(ABC_CC_Reinitialization,
                         "ABC_Sync has already been initalized");

    ABC_CHECK_GIT(git_libgit2_init());
    gbInitialized = true;

    if (szCaCertPath)
        ABC_CHECK_GIT(git_libgit2_opts(GIT_OPT_SET_SSL_CERT_LOCATIONS, szCaCertPath,
                                       nullptr));

    // Choose a random server to start with:
    syncServerIndex = time(nullptr);

    return Status();
}

void
syncTerminate()
{
    AutoSyncLock lock(gSyncMutex);

    if (gbInitialized)
    {
        git_libgit2_shutdown();
        gbInitialized = false;
    }
}

Status
syncMakeRepo(const std::string &syncDir)
{
    AutoSyncLock lock(gSyncMutex);

    git_repository_init_options opts = GIT_REPOSITORY_INIT_OPTIONS_INIT;
    opts.flags |= GIT_REPOSITORY_INIT_MKDIR;
    opts.flags |= GIT_REPOSITORY_INIT_MKPATH;

    AutoFree<git_repository, git_repository_free> repo;
    ABC_CHECK_GIT(git_repository_init_ext(&repo.get(), syncDir.c_str(), &opts));

    return Status();
}

Status
syncEnsureRepo(const std::string &syncDir, const std::string &tempDir,
               const std::string &syncKey)
{
    AutoSyncLock lock(gSyncMutex);

    if (!fileExists(syncDir))
    {
        if (fileExists(tempDir))
            ABC_CHECK(fileDelete(tempDir));
        ABC_CHECK(syncMakeRepo(tempDir));
        bool dirty = false;
        ABC_CHECK(syncRepo(tempDir, syncKey, dirty));
        if (rename(tempDir.c_str(), syncDir.c_str()))
            return ABC_ERROR(ABC_CC_SysError, "rename failed");
    }

    return Status();
}

Status
syncRepo(const std::string &syncDir, const std::string &syncKey, bool &dirty)
{
    AutoSyncLock lock(gSyncMutex);

    AutoFree<git_repository, git_repository_free> repo;
    ABC_CHECK_GIT(git_repository_open(&repo.get(), syncDir.c_str()));

    std::string url;

    ABC_CHECK(syncUrl(url, syncKey));

    for (int i = 0; i < syncServers.size(); i++)
    {
        ABC_CHECK(syncUrl(url, syncKey, true));
        if (sync_fetch(repo, url.c_str()) >= 0)
        {
            ABC_DebugLog("Syncing to: %s", url.c_str());
            break;
        }
        else
        {
            ABC_DebugLog("FAIlED Syncing to: %s", url.c_str());
        }
    }

    int files_changed, need_push;
    ABC_CHECK_GIT(sync_master(repo, &files_changed, &need_push));

    if (need_push)
        ABC_CHECK_GIT(sync_push(repo, url.c_str()));

    // If this fails, the app has been shut down, leaving us for dead.
    // We will crash anyhow, but this at least makes it official:
    assert(gContext);

    dirty = !!files_changed;
    return Status();
}

} // namespace abcd
