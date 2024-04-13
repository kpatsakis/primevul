static struct node *lookup_node_by_id_locked(struct fuse *fuse, __u64 nid)
{
 if (nid == FUSE_ROOT_ID) {
 return &fuse->global->root;
 } else {
 return id_to_ptr(nid);
 }
}
