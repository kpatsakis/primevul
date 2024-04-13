static void nfs4_locku_done(struct rpc_task *task, void *data)
{
	struct nfs4_unlockdata *calldata = data;

	if (RPC_ASSASSINATED(task))
		return;
	switch (task->tk_status) {
		case 0:
			memcpy(calldata->lsp->ls_stateid.data,
					calldata->res.stateid.data,
					sizeof(calldata->lsp->ls_stateid.data));
			renew_lease(calldata->server, calldata->timestamp);
			break;
		case -NFS4ERR_BAD_STATEID:
		case -NFS4ERR_OLD_STATEID:
		case -NFS4ERR_STALE_STATEID:
		case -NFS4ERR_EXPIRED:
			break;
		default:
			if (nfs4_async_handle_error(task, calldata->server, NULL) == -EAGAIN)
				rpc_restart_call(task);
	}
}
