struct nfs4_state_owner *nfs4_get_state_owner(struct nfs_server *server, struct rpc_cred *cred)
{
	struct nfs_client *clp = server->nfs_client;
	struct nfs4_state_owner *sp, *new;

	spin_lock(&clp->cl_lock);
	sp = nfs4_find_state_owner(server, cred);
	spin_unlock(&clp->cl_lock);
	if (sp != NULL)
		return sp;
	new = nfs4_alloc_state_owner();
	if (new == NULL)
		return NULL;
	new->so_client = clp;
	new->so_server = server;
	new->so_cred = cred;
	spin_lock(&clp->cl_lock);
	sp = nfs4_insert_state_owner(clp, new);
	spin_unlock(&clp->cl_lock);
	if (sp == new)
		get_rpccred(cred);
	else {
		rpc_destroy_wait_queue(&new->so_sequence.wait);
		kfree(new);
	}
	return sp;
}
