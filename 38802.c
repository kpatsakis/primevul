static void nfs4_locku_prepare(struct rpc_task *task, void *data)
{
	struct nfs4_unlockdata *calldata = data;

	if (nfs_wait_on_sequence(calldata->arg.seqid, task) != 0)
		return;
	if (test_bit(NFS_LOCK_INITIALIZED, &calldata->lsp->ls_flags) == 0) {
		/* Note: exit _without_ running nfs4_locku_done */
		task->tk_action = NULL;
		return;
	}
	calldata->timestamp = jiffies;
	if (nfs4_setup_sequence(calldata->server,
				&calldata->arg.seq_args,
				&calldata->res.seq_res,
				task) != 0)
		nfs_release_seqid(calldata->arg.seqid);
	else
		rpc_call_start(task);
}
