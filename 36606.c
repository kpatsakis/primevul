static int nfs4_check_lease(struct nfs_client *clp)
{
	struct rpc_cred *cred;
	int status = -NFS4ERR_EXPIRED;

	/* Is the client already known to have an expired lease? */
	if (test_bit(NFS4CLNT_LEASE_EXPIRED, &clp->cl_state))
		return 0;
	cred = nfs4_get_renew_cred(clp);
	if (cred == NULL) {
		cred = nfs4_get_setclientid_cred(clp);
		if (cred == NULL)
			goto out;
	}
	status = nfs4_proc_renew(clp, cred);
	put_rpccred(cred);
out:
	nfs4_recovery_handle_error(clp, status);
	return status;
}
