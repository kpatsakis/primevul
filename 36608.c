static void nfs4_clear_state_manager_bit(struct nfs_client *clp)
{
	smp_mb__before_clear_bit();
	clear_bit(NFS4CLNT_MANAGER_RUNNING, &clp->cl_state);
	smp_mb__after_clear_bit();
	wake_up_bit(&clp->cl_state, NFS4CLNT_MANAGER_RUNNING);
	rpc_wake_up(&clp->cl_rpcwaitq);
}
