static void fuse_status(struct fuse *fuse, __u64 unique, int err)
{
 struct fuse_out_header hdr;
    hdr.len = sizeof(hdr);
    hdr.error = err;
    hdr.unique = unique;
    write(fuse->fd, &hdr, sizeof(hdr));
}
