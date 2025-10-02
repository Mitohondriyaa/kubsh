#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <errno.h>
#include "vfs.h"

int can_login(struct passwd* pwd) {
    FILE* shells = fopen("/etc/shells", "r");

    if (!shells) {
        fprintf(stderr, "\033[1;31mCritical filesystem error\033[0m");
        exit(EXIT_FAILURE);
    }

    char line[256];

    while (fgets(line, sizeof(line), shells)) {
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, pwd->pw_shell) == 0) {
            fclose(shells);
            return 1;
        }
    }

    fclose(shells);
    return 0;
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
        
        return -ENOENT;
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

int users_readdir(
    const char* path,
    void* buf, 
    fuse_fill_dir_t filler, 
    off_t offset, 
    struct fuse_file_info* fi, 
    enum fuse_readdir_flags flags
) {
    (void) offset;
    (void) fi;
    (void) flags;

    filler(buf, ".", NULL, 0, 0);
    filler(buf, "..", NULL, 0, 0);

    if (strcmp(path, "/") == 0) {
        struct passwd* pwd;
        setpwent();

        while ((pwd = getpwent()) != NULL) {
            if (can_login(pwd)) {
                filler(buf, pwd->pw_name, NULL, 0, 0);
            }
        }

        endpwent();
        return 0;
    }

    char username[256];

    if (sscanf(path, "/%255[^/]", username) == 1) {
        filler(buf, "id", NULL, 0, 0);
        filler(buf, "home", NULL, 0, 0);
        filler(buf, "shell", NULL, 0, 0);
        
        return 0;
    }

    return -ENOENT;
}

struct fuse_operations users_operations = {
    .getattr = users_getattr,
    .readdir = users_readdir
};