static struct rpc_cred *nfs4_get_setclientid_cred(struct nfs_client *clp)
{
	struct nfs4_state_owner *sp;
	struct rb_node *pos;
	struct rpc_cred *cred;

	spin_lock(&clp->cl_lock);
	cred = nfs4_get_machine_cred_locked(clp);
	if (cred != NULL)
		goto out;
	pos = rb_first(&clp->cl_state_owners);
	if (pos != NULL) {
		sp = rb_entry(pos, struct nfs4_state_owner, so_client_node);
		cred = get_rpccred(sp->so_cred);
	}
out:
	spin_unlock(&clp->cl_lock);
	return cred;
}
