static int nfs4_commit_done(struct rpc_task *task, struct nfs_write_data *data)
{
	struct inode *inode = data->inode;
	
	if (nfs4_async_handle_error(task, NFS_SERVER(inode), NULL) == -EAGAIN) {
		rpc_restart_call(task);
		return -EAGAIN;
	}
	nfs_refresh_inode(inode, data->res.fattr);
	return 0;
}
