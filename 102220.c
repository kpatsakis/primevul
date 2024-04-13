static int handle_rename(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header* hdr, const struct fuse_rename_in* req,
 const char* old_name, const char* new_name)
{
 struct node* old_parent_node;
 struct node* new_parent_node;
 struct node* child_node;
 char old_parent_path[PATH_MAX];
 char new_parent_path[PATH_MAX];
 char old_child_path[PATH_MAX];
 char new_child_path[PATH_MAX];
 const char* new_actual_name;
 int res;

    pthread_mutex_lock(&fuse->global->lock);
    old_parent_node = lookup_node_and_path_by_id_locked(fuse, hdr->nodeid,
            old_parent_path, sizeof(old_parent_path));
    new_parent_node = lookup_node_and_path_by_id_locked(fuse, req->newdir,
            new_parent_path, sizeof(new_parent_path));
    TRACE("[%d] RENAME %s->%s @ %"PRIx64" (%s) -> %"PRIx64" (%s)\n", handler->token,
            old_name, new_name,
            hdr->nodeid, old_parent_node ? old_parent_node->name : "?",
            req->newdir, new_parent_node ? new_parent_node->name : "?");
 if (!old_parent_node || !new_parent_node) {
        res = -ENOENT;
 goto lookup_error;
 }
 if (!check_caller_access_to_name(fuse, hdr, old_parent_node, old_name, W_OK)) {
        res = -EACCES;
 goto lookup_error;
 }
 if (!check_caller_access_to_name(fuse, hdr, new_parent_node, new_name, W_OK)) {
        res = -EACCES;
 goto lookup_error;
 }
    child_node = lookup_child_by_name_locked(old_parent_node, old_name);
 if (!child_node || get_node_path_locked(child_node,
            old_child_path, sizeof(old_child_path)) < 0) {
        res = -ENOENT;
 goto lookup_error;
 }
    acquire_node_locked(child_node);
    pthread_mutex_unlock(&fuse->global->lock);

 /* Special case for renaming a file where destination is same path
     * differing only by case.  In this case we don't want to look for a case
     * insensitive match.  This allows commands like "mv foo FOO" to work as expected.
     */
 int search = old_parent_node != new_parent_node
 || strcasecmp(old_name, new_name);
 if (!(new_actual_name = find_file_within(new_parent_path, new_name,
            new_child_path, sizeof(new_child_path), search))) {
        res = -ENOENT;
 goto io_error;
 }

    TRACE("[%d] RENAME %s->%s\n", handler->token, old_child_path, new_child_path);
    res = rename(old_child_path, new_child_path);
 if (res < 0) {
        res = -errno;
 goto io_error;
 }

    pthread_mutex_lock(&fuse->global->lock);
    res = rename_node_locked(child_node, new_name, new_actual_name);
 if (!res) {
        remove_node_from_parent_locked(child_node);
        derive_permissions_locked(fuse, new_parent_node, child_node);
        derive_permissions_recursive_locked(fuse, child_node);
        add_node_to_parent_locked(child_node, new_parent_node);
 }
 goto done;

io_error:
    pthread_mutex_lock(&fuse->global->lock);
done:
    release_node_locked(child_node);
lookup_error:
    pthread_mutex_unlock(&fuse->global->lock);
 return res;
}
