nfs4_insert_state_owner(struct nfs_client *clp, struct nfs4_state_owner *new)
{
	struct rb_node **p = &clp->cl_state_owners.rb_node,
		       *parent = NULL;
	struct nfs4_state_owner *sp;

	while (*p != NULL) {
		parent = *p;
		sp = rb_entry(parent, struct nfs4_state_owner, so_client_node);

		if (new->so_server < sp->so_server) {
			p = &parent->rb_left;
			continue;
		}
		if (new->so_server > sp->so_server) {
			p = &parent->rb_right;
			continue;
		}
		if (new->so_cred < sp->so_cred)
			p = &parent->rb_left;
		else if (new->so_cred > sp->so_cred)
			p = &parent->rb_right;
		else {
			atomic_inc(&sp->so_count);
			return sp;
		}
	}
	nfs_alloc_unique_id(&clp->cl_openowner_id, &new->so_owner_id, 1, 64);
	rb_link_node(&new->so_client_node, parent, p);
	rb_insert_color(&new->so_client_node, &clp->cl_state_owners);
	return new;
}
