static int local_opendir(FsContext *ctx,
                         V9fsPath *fs_path, V9fsFidOpenState *fs)
{
    int dirfd;
    DIR *stream;

    dirfd = local_opendir_nofollow(ctx, fs_path->data);
    if (dirfd == -1) {
        return -1;
    }

    stream = fdopendir(dirfd);
    if (!stream) {
        close(dirfd);
        return -1;
    }
    fs->dir.stream = stream;
    return 0;
}
