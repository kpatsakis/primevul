static int local_remove(FsContext *ctx, const char *path)
{
    struct stat stbuf;
    char *dirpath = g_path_get_dirname(path);
    char *name = g_path_get_basename(path);
    int flags = 0;
    int dirfd;
    int err = -1;

    dirfd = local_opendir_nofollow(ctx, dirpath);
    if (dirfd == -1) {
        goto out;
    }

    if (fstatat(dirfd, path, &stbuf, AT_SYMLINK_NOFOLLOW) < 0) {
        goto err_out;
    }

    if (S_ISDIR(stbuf.st_mode)) {
        flags |= AT_REMOVEDIR;
    }

    err = local_unlinkat_common(ctx, dirfd, name, flags);
err_out:
    close_preserve_errno(dirfd);
out:
    g_free(name);
    g_free(dirpath);
    return err;
}
