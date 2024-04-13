static void nfs4_recovery_handle_error(struct nfs_client *clp, int error)
{
	switch (error) {
		case -NFS4ERR_CB_PATH_DOWN:
			nfs_handle_cb_pathdown(clp);
			break;
		case -NFS4ERR_STALE_CLIENTID:
		case -NFS4ERR_LEASE_MOVED:
			set_bit(NFS4CLNT_LEASE_EXPIRED, &clp->cl_state);
			nfs4_state_start_reclaim_reboot(clp);
			break;
		case -NFS4ERR_EXPIRED:
			set_bit(NFS4CLNT_LEASE_EXPIRED, &clp->cl_state);
			nfs4_state_start_reclaim_nograce(clp);
	}
}
