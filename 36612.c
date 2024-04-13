nfs4_find_state_owner(struct nfs_server *server, struct rpc_cred *cred)
{
	struct nfs_client *clp = server->nfs_client;
	struct rb_node **p = &clp->cl_state_owners.rb_node,
		       *parent = NULL;
	struct nfs4_state_owner *sp, *res = NULL;

	while (*p != NULL) {
		parent = *p;
		sp = rb_entry(parent, struct nfs4_state_owner, so_client_node);

		if (server < sp->so_server) {
			p = &parent->rb_left;
			continue;
		}
		if (server > sp->so_server) {
			p = &parent->rb_right;
			continue;
		}
		if (cred < sp->so_cred)
			p = &parent->rb_left;
		else if (cred > sp->so_cred)
			p = &parent->rb_right;
		else {
			atomic_inc(&sp->so_count);
			res = sp;
			break;
		}
	}
	return res;
}
