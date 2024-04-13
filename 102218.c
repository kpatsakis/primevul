static int handle_mkdir(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header* hdr, const struct fuse_mkdir_in* req, const char* name)
{
 struct node* parent_node;
 char parent_path[PATH_MAX];
 char child_path[PATH_MAX];
 const char* actual_name;

    pthread_mutex_lock(&fuse->global->lock);
    parent_node = lookup_node_and_path_by_id_locked(fuse, hdr->nodeid,
            parent_path, sizeof(parent_path));
    TRACE("[%d] MKDIR %s 0%o @ %"PRIx64" (%s)\n", handler->token,
            name, req->mode, hdr->nodeid, parent_node ? parent_node->name : "?");
    pthread_mutex_unlock(&fuse->global->lock);

 if (!parent_node || !(actual_name = find_file_within(parent_path, name,
            child_path, sizeof(child_path), 1))) {
 return -ENOENT;
 }
 if (!check_caller_access_to_name(fuse, hdr, parent_node, name, W_OK)) {
 return -EACCES;
 }
    __u32 mode = (req->mode & (~0777)) | 0775;
 if (mkdir(child_path, mode) < 0) {
 return -errno;
 }

 /* When creating /Android/data and /Android/obb, mark them as .nomedia */
 if (parent_node->perm == PERM_ANDROID && !strcasecmp(name, "data")) {
 char nomedia[PATH_MAX];
        snprintf(nomedia, PATH_MAX, "%s/.nomedia", child_path);
 if (touch(nomedia, 0664) != 0) {
            ERROR("Failed to touch(%s): %s\n", nomedia, strerror(errno));
 return -ENOENT;
 }
 }
 if (parent_node->perm == PERM_ANDROID && !strcasecmp(name, "obb")) {
 char nomedia[PATH_MAX];
        snprintf(nomedia, PATH_MAX, "%s/.nomedia", fuse->global->obb_path);
 if (touch(nomedia, 0664) != 0) {
            ERROR("Failed to touch(%s): %s\n", nomedia, strerror(errno));
 return -ENOENT;
 }
 }

 return fuse_reply_entry(fuse, hdr->unique, parent_node, name, actual_name, child_path);
}
