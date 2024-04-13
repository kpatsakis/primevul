ssize_t local_getxattr_nofollow(FsContext *ctx, const char *path,
                                const char *name, void *value, size_t size)
{
    char *dirpath = g_path_get_dirname(path);
    char *filename = g_path_get_basename(path);
    int dirfd;
    ssize_t ret = -1;

    dirfd = local_opendir_nofollow(ctx, dirpath);
    if (dirfd == -1) {
        goto out;
    }

    ret = fgetxattrat_nofollow(dirfd, filename, name, value, size);
    close_preserve_errno(dirfd);
out:
    g_free(dirpath);
    g_free(filename);
    return ret;
}
