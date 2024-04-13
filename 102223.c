static struct node* lookup_node_and_path_by_id_locked(struct fuse* fuse, __u64 nid,
 char* buf, size_t bufsize)
{
 struct node* node = lookup_node_by_id_locked(fuse, nid);
 if (node && get_node_path_locked(node, buf, bufsize) < 0) {
        node = NULL;
 }
 return node;
}
