void nfs4_schedule_state_recovery(struct nfs_client *clp)
{
	if (!clp)
		return;
	if (!test_bit(NFS4CLNT_LEASE_EXPIRED, &clp->cl_state))
		set_bit(NFS4CLNT_CHECK_LEASE, &clp->cl_state);
	nfs4_schedule_state_manager(clp);
}
