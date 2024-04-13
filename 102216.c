static int handle_forget(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header *hdr, const struct fuse_forget_in *req)
{
 struct node* node;

    pthread_mutex_lock(&fuse->global->lock);
    node = lookup_node_by_id_locked(fuse, hdr->nodeid);
    TRACE("[%d] FORGET #%"PRIu64" @ %"PRIx64" (%s)\n", handler->token, req->nlookup,
            hdr->nodeid, node ? node->name : "?");
 if (node) {
        __u64 n = req->nlookup;
 while (n--) {
            release_node_locked(node);
 }
 }
    pthread_mutex_unlock(&fuse->global->lock);
 return NO_STATUS; /* no reply */
}
