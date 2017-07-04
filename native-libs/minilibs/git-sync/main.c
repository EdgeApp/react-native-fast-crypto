#include "sync.h"
#include <stdio.h>

static void print_error()
{
    const git_error *error = giterr_last();
    if (error && error->message)
        fprintf(stderr, "libgit2 error: %s\n", error->message);
}

int main(int argc, char *argv[])
{
    int rv = 0;
    git_repository *repo = NULL;
    int files_changed = 0;
    int need_push = 0;

    if (git_libgit2_init() < 0)
    {
        fprintf(stderr, "error: git_threads_init failed\n");
        return 1;
    }

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <local-path> <remote-uri>\n", argv[0]);
        rv = 1;
        goto exit;
    }

    if (git_repository_open(&repo, argv[1]) < 0)
    {
        print_error();
        fprintf(stderr, "error: cannot open repository '%s'\n", argv[1]);
        rv = 1;
        goto exit;
    }

    if (sync_fetch(repo, argv[2]) < 0)
    {
        print_error();
        fprintf(stderr, "error: cannot fetch from '%s'\n", argv[2]);
        rv = 1;
        goto exit;
    }

    if (sync_master(repo, &files_changed, &need_push) < 0)
    {
        print_error();
        fprintf(stderr, "error: failed to merge\n");
        rv = 1;
        goto exit;
    }

    if (need_push && sync_push(repo, argv[2]) < 0)
    {
        print_error();
        fprintf(stderr, "error: cannot push to '%s'\n", argv[2]);
        rv = 1;
        goto exit;
    }

exit:
    if (repo)           git_repository_free(repo);
    git_libgit2_shutdown();
    return rv;
}
