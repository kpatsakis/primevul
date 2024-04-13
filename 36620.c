static struct rpc_cred *nfs4_get_renew_cred(struct nfs_client *clp)
{
	struct rpc_cred *cred;

	spin_lock(&clp->cl_lock);
	cred = nfs4_get_renew_cred_locked(clp);
	spin_unlock(&clp->cl_lock);
	return cred;
}
