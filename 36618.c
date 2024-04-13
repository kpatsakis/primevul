static struct rpc_cred *nfs4_get_machine_cred_locked(struct nfs_client *clp)
{
	struct rpc_cred *cred = NULL;

	if (clp->cl_machine_cred != NULL)
		cred = get_rpccred(clp->cl_machine_cred);
	return cred;
}
