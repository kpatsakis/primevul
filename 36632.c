nfs4_remove_state_owner(struct nfs_client *clp, struct nfs4_state_owner *sp)
{
	if (!RB_EMPTY_NODE(&sp->so_client_node))
		rb_erase(&sp->so_client_node, &clp->cl_state_owners);
	nfs_free_unique_id(&clp->cl_openowner_id, &sp->so_owner_id);
}
