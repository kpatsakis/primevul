static void nfs4_open_confirm_done(struct rpc_task *task, void *calldata)
{
	struct nfs4_opendata *data = calldata;

	data->rpc_status = task->tk_status;
	if (RPC_ASSASSINATED(task))
		return;
	if (data->rpc_status == 0) {
		memcpy(data->o_res.stateid.data, data->c_res.stateid.data,
				sizeof(data->o_res.stateid.data));
		nfs_confirm_seqid(&data->owner->so_seqid, 0);
		renew_lease(data->o_res.server, data->timestamp);
		data->rpc_done = 1;
	}
}
