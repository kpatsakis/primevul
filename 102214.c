static bool check_caller_access_to_node(struct fuse* fuse,
 const struct fuse_in_header *hdr, const struct node* node, int mode) {
 return check_caller_access_to_name(fuse, hdr, node->parent, node->name, mode);
}
