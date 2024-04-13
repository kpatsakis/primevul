tlink_rb_search(struct rb_root *root, uid_t uid)
{
	struct rb_node *node = root->rb_node;
	struct tcon_link *tlink;

	while (node) {
		tlink = rb_entry(node, struct tcon_link, tl_rbnode);

		if (tlink->tl_uid > uid)
			node = node->rb_left;
		else if (tlink->tl_uid < uid)
			node = node->rb_right;
		else
			return tlink;
	}
	return NULL;
}
