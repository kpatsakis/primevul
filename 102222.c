static int handle_unlink(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header* hdr, const char* name)
{
 struct node* parent_node;
 struct node* child_node;
 char parent_path[PATH_MAX];
 char child_path[PATH_MAX];

    pthread_mutex_lock(&fuse->global->lock);
    parent_node = lookup_node_and_path_by_id_locked(fuse, hdr->nodeid,
            parent_path, sizeof(parent_path));
    TRACE("[%d] UNLINK %s @ %"PRIx64" (%s)\n", handler->token,
            name, hdr->nodeid, parent_node ? parent_node->name : "?");
    pthread_mutex_unlock(&fuse->global->lock);

 if (!parent_node || !find_file_within(parent_path, name,
            child_path, sizeof(child_path), 1)) {
 return -ENOENT;
 }
 if (!check_caller_access_to_name(fuse, hdr, parent_node, name, W_OK)) {
 return -EACCES;
 }
 if (unlink(child_path) < 0) {
 return -errno;
 }
    pthread_mutex_lock(&fuse->global->lock);
    child_node = lookup_child_by_name_locked(parent_node, name);
 if (child_node) {
        child_node->deleted = true;
 }
    pthread_mutex_unlock(&fuse->global->lock);
 if (parent_node && child_node) {
 /* Tell all other views that node is gone */
        TRACE("[%d] fuse_notify_delete parent=%"PRIx64", child=%"PRIx64", name=%s\n",
                handler->token, (uint64_t) parent_node->nid, (uint64_t) child_node->nid, name);
 if (fuse != fuse->global->fuse_default) {
            fuse_notify_delete(fuse->global->fuse_default, parent_node->nid, child_node->nid, name);
 }
 if (fuse != fuse->global->fuse_read) {
            fuse_notify_delete(fuse->global->fuse_read, parent_node->nid, child_node->nid, name);
 }
 if (fuse != fuse->global->fuse_write) {
            fuse_notify_delete(fuse->global->fuse_write, parent_node->nid, child_node->nid, name);
 }
 }
 return 0;
}
