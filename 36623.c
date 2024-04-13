static int nfs4_init_client(struct nfs_client *clp, struct rpc_cred *cred)
{
	int status = nfs4_proc_setclientid(clp, NFS4_CALLBACK,
			nfs_callback_tcpport, cred);
	if (status == 0)
		status = nfs4_proc_setclientid_confirm(clp, cred);
	if (status == 0)
		nfs4_schedule_state_renewal(clp);
	return status;
}
