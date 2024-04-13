static void nfs_free_unique_id(struct rb_root *root, struct nfs_unique_id *id)
{
	rb_erase(&id->rb_node, root);
}
