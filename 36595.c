static void nfs4_renew_done(struct rpc_task *task, void *data)
{
	struct nfs_client *clp = (struct nfs_client *)task->tk_msg.rpc_argp;
	unsigned long timestamp = (unsigned long)data;

	if (task->tk_status < 0) {
		switch (task->tk_status) {
			case -NFS4ERR_STALE_CLIENTID:
			case -NFS4ERR_EXPIRED:
			case -NFS4ERR_CB_PATH_DOWN:
				nfs4_schedule_state_recovery(clp);
		}
		return;
	}
	spin_lock(&clp->cl_lock);
	if (time_before(clp->cl_last_renewal,timestamp))
		clp->cl_last_renewal = timestamp;
	spin_unlock(&clp->cl_lock);
}
