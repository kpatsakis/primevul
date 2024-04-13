static void nfs4_delegreturn_done(struct rpc_task *task, void *calldata)
{
	struct nfs4_delegreturndata *data = calldata;
	data->rpc_status = task->tk_status;
	if (data->rpc_status == 0)
		renew_lease(data->res.server, data->timestamp);
}
