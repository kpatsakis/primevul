static int nfs4_recover_expired_lease(struct nfs_server *server)
{
	struct nfs_client *clp = server->nfs_client;
	int ret;

	for (;;) {
		ret = nfs4_wait_clnt_recover(clp);
		if (ret != 0)
			return ret;
		if (!test_bit(NFS4CLNT_LEASE_EXPIRED, &clp->cl_state) &&
		    !test_bit(NFS4CLNT_CHECK_LEASE,&clp->cl_state))
			break;
		nfs4_schedule_state_recovery(clp);
	}
	return 0;
}
