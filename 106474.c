static int local_statfs(FsContext *s, V9fsPath *fs_path, struct statfs *stbuf)
{
    int fd, ret;

    fd = local_open_nofollow(s, fs_path->data, O_RDONLY, 0);
    if (fd == -1) {
        return -1;
    }
    ret = fstatfs(fd, stbuf);
    close_preserve_errno(fd);
    return ret;
}
