#include "sync.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define REPO_A "test/a"
#define REPO_B "test/b"
#define SERVER "test/server.git"

#define CHECK(f) \
if ((e = f) < 0) \
{ \
    print_error(); \
    fprintf(stderr, "error on line %d: %s\n", __LINE__, #f); \
    goto exit; \
}

static void print_error()
{
    const git_error *error = giterr_last();
    if (error && error->message)
        fprintf(stderr, "libgit2 error: %s\n", error->message);
}

static int create_file(const char *path, const char *contents)
{
    FILE *file = fopen(path, "w");
    if (!file)
        return -1;

    if (1 != fwrite(contents, strlen(contents), 1, file))
    {
        fclose(file);
        return -1;
    }
    fclose(file);
    return 0;
}

static int do_sync(git_repository *repo, const char *server)
{
    int e = 0;
    int dirty, need_push;

    CHECK(sync_fetch(repo, server));
    CHECK(sync_master(repo, &dirty, &need_push));
    if (need_push)
        CHECK(sync_push(repo, server));

exit:
    return e;
}

int main(int argc, char *argv[])
{
    int e = 0;
    git_repository *repo_a = NULL;
    git_repository *repo_b = NULL;
    git_repository *server = NULL;

    CHECK(git_libgit2_init());

    CHECK(mkdir("test", S_IRWXU | S_IRWXG | S_IRWXO));
    CHECK(mkdir(REPO_A, S_IRWXU | S_IRWXG | S_IRWXO));
    CHECK(mkdir(REPO_B, S_IRWXU | S_IRWXG | S_IRWXO));
    CHECK(mkdir(SERVER, S_IRWXU | S_IRWXG | S_IRWXO));

    CHECK(git_repository_init(&repo_a, REPO_A, 0));
    CHECK(git_repository_init(&repo_b, REPO_B, 0));
    CHECK(git_repository_init(&server, SERVER, 1));

    // Start repo a:
    CHECK(create_file(REPO_A "/a.txt", "a\n"));
    CHECK(do_sync(repo_a, SERVER));

    // Start repo b:
    CHECK(create_file(REPO_B "/b.txt", "b\n"));
    CHECK(do_sync(repo_b, SERVER));
    CHECK(do_sync(repo_a, SERVER));

    // Create a conflict:
    CHECK(remove(REPO_A "/a.txt"));
    CHECK(create_file(REPO_A "/c.txt", "a\n"));
    CHECK(create_file(REPO_B "/c.txt", "b\n"));
    CHECK(do_sync(repo_a, SERVER));
    CHECK(do_sync(repo_b, SERVER));
    CHECK(do_sync(repo_a, SERVER));

    // Create a subdir:
    CHECK(mkdir(REPO_A "/sub", S_IRWXU | S_IRWXG | S_IRWXO));
    CHECK(create_file(REPO_A "/sub/a.txt", "a\n"));
    CHECK(do_sync(repo_a, SERVER));
    CHECK(do_sync(repo_b, SERVER));

    // Subdir chaos:
    CHECK(create_file(REPO_B "/sub/b.txt", "b\n"));
    CHECK(remove(REPO_A "/sub/a.txt"));
    CHECK(create_file(REPO_A "/sub/c.txt", "a\n"));
    CHECK(create_file(REPO_B "/sub/c.txt", "b\n"));
    CHECK(do_sync(repo_a, SERVER));
    CHECK(do_sync(repo_b, SERVER));
    CHECK(do_sync(repo_a, SERVER));

    // When this is done, the two subdirs should match exactly:
    // b.txt = b
    // c.txt = a
    // sub/b.txt = b
    // sub/c.txt = a

    // TODO: Verify this in code

exit:
    if (repo_a) git_repository_free(repo_a);
    if (repo_b) git_repository_free(repo_b);
    if (server) git_repository_free(server);
    git_libgit2_shutdown();

    return e;
}
