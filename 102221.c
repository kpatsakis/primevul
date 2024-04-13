static int handle_statfs(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header* hdr)
{
 char path[PATH_MAX];
 struct statfs stat;
 struct fuse_statfs_out out;
 int res;

    pthread_mutex_lock(&fuse->global->lock);
    TRACE("[%d] STATFS\n", handler->token);
    res = get_node_path_locked(&fuse->global->root, path, sizeof(path));
    pthread_mutex_unlock(&fuse->global->lock);
 if (res < 0) {
 return -ENOENT;
 }
 if (statfs(fuse->global->root.name, &stat) < 0) {
 return -errno;
 }
    memset(&out, 0, sizeof(out));
    out.st.blocks = stat.f_blocks;
    out.st.bfree = stat.f_bfree;
    out.st.bavail = stat.f_bavail;
    out.st.files = stat.f_files;
    out.st.ffree = stat.f_ffree;
    out.st.bsize = stat.f_bsize;
    out.st.namelen = stat.f_namelen;
    out.st.frsize = stat.f_frsize;
    fuse_reply(fuse, hdr->unique, &out, sizeof(out));
 return NO_STATUS;
}
