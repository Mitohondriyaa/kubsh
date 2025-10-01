#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <errno.h>
#include "vfs.h"

int can_login(struct passwd* pwd) {
    return pwd
           && pwd->pw_shell
           && strlen(pwd->pw_shell) > 0
           && strcmp(pwd->pw_shell, "/bin/false") != 0
           && strcmp(pwd->pw_shell, "/usr/sbin/nologin") != 0
           && strcmp(pwd->pw_shell, "/sbin/nologin") != 0;
}

int users_getattr(const char* path, struct stat* st, struct fuse_file_info* fi) {
    (void) fi;

    memset(st, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0) {
        st->st_mode = __S_IFDIR | 0755;
        st->st_nlink = 2;
        st->st_uid = getuid();
        st->st_gid = getgid();
        st->st_size = 4096;
        st->st_blksize = 4096;
        st->st_blocks = 8;

        return 0;
    }

    char username[256];
    char filename[256];

    if (sscanf(path, "/%255[^/]/%255[^/]", username, filename) == 2) {
        struct passwd* pwd = getpwnam(username);

        if (pwd && can_login(pwd)) {
            if (
                strcmp(filename, "id") == 0
                || strcmp(filename, "home") == 0
                || strcmp(filename, "shell") == 0
            ) {
                st->st_mode = __S_IFREG | 0644;
                st->st_nlink = 1;
                st->st_uid = pwd->pw_uid;
                st->st_gid = pwd->pw_gid;

                if (strcmp(filename, "id") == 0) {
                    st->st_size = snprintf(NULL, 0, "%d", pwd->pw_uid);
                }
                else if (strcmp(filename, "home") == 0) {
                    st->st_size = strlen(pwd->pw_dir);
                }
                else {
                    st->st_size = strlen(pwd->pw_shell);
                }

                st->st_blksize = 4096;
                st->st_blocks = (st->st_size + 511) / 512;

                return 0;
            }
        }
    }

    if (sscanf(path, "/%255[^/]", username) == 1) {
        struct passwd* pwd = getpwnam(username);

        if (pwd && can_login(pwd)) {
            st->st_mode = __S_IFDIR | 0755;
            st->st_nlink = 2;
            st->st_uid = pwd->pw_uid;
            st->st_gid = pwd->pw_gid;
            st->st_size = 4096;
            st->st_blksize = 4096;
            st->st_blocks = 8;

            return 0;
        }
    }

    return -ENOENT;
}

struct fuse_operations users_operations = {
    .getattr = users_getattr
};