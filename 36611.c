nfs4_drop_state_owner(struct nfs4_state_owner *sp)
{
	if (!RB_EMPTY_NODE(&sp->so_client_node)) {
		struct nfs_client *clp = sp->so_client;

		spin_lock(&clp->cl_lock);
		rb_erase(&sp->so_client_node, &clp->cl_state_owners);
		RB_CLEAR_NODE(&sp->so_client_node);
		spin_unlock(&clp->cl_lock);
	}
}
