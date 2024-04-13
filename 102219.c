static int handle_release(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header* hdr, const struct fuse_release_in* req)
{
 struct handle *h = id_to_ptr(req->fh);

    TRACE("[%d] RELEASE %p(%d)\n", handler->token, h, h->fd);
    close(h->fd);
    free(h);
 return 0;
}
