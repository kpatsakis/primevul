static void nfs4_lock_done(struct rpc_task *task, void *calldata)
{
	struct nfs4_lockdata *data = calldata;

	dprintk("%s: begin!\n", __func__);

	data->rpc_status = task->tk_status;
	if (RPC_ASSASSINATED(task))
		goto out;
	if (data->arg.new_lock_owner != 0) {
		if (data->rpc_status == 0)
			nfs_confirm_seqid(&data->lsp->ls_seqid, 0);
		else
			goto out;
	}
	if (data->rpc_status == 0) {
		memcpy(data->lsp->ls_stateid.data, data->res.stateid.data,
					sizeof(data->lsp->ls_stateid.data));
		data->lsp->ls_flags |= NFS_LOCK_INITIALIZED;
		renew_lease(NFS_SERVER(data->ctx->path.dentry->d_inode), data->timestamp);
	}
out:
	dprintk("%s: done, ret = %d!\n", __func__, data->rpc_status);
}
