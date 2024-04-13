static int nfs4_reclaim_lease(struct nfs_client *clp)
{
	struct rpc_cred *cred;
	int status = -ENOENT;

	cred = nfs4_get_setclientid_cred(clp);
	if (cred != NULL) {
		status = nfs4_init_client(clp, cred);
		put_rpccred(cred);
		/* Handle case where the user hasn't set up machine creds */
		if (status == -EACCES && cred == clp->cl_machine_cred) {
			nfs4_clear_machine_cred(clp);
			status = -EAGAIN;
		}
	}
	return status;
}
