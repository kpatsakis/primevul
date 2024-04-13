static int nfs4_read_done(struct rpc_task *task, struct nfs_read_data *data)
{
	struct nfs_server *server = NFS_SERVER(data->inode);

	if (nfs4_async_handle_error(task, server, data->args.context->state) == -EAGAIN) {
		rpc_restart_call(task);
		return -EAGAIN;
	}

	nfs_invalidate_atime(data->inode);
	if (task->tk_status > 0)
		renew_lease(server, data->timestamp);
	return 0;
}
