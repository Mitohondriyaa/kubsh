#ifndef VFS_H
#define VFS_H

#define FUSE_USE_VERSION 34

#include <fuse3/fuse.h>

extern struct fuse_operations users_operations;

#endif