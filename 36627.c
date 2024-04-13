void nfs4_put_state_owner(struct nfs4_state_owner *sp)
{
	struct nfs_client *clp = sp->so_client;
	struct rpc_cred *cred = sp->so_cred;

	if (!atomic_dec_and_lock(&sp->so_count, &clp->cl_lock))
		return;
	nfs4_remove_state_owner(clp, sp);
	spin_unlock(&clp->cl_lock);
	rpc_destroy_wait_queue(&sp->so_sequence.wait);
	put_rpccred(cred);
	kfree(sp);
}
