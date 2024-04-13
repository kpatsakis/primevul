static int nfs4_proc_unlink_done(struct rpc_task *task, struct inode *dir)
{
	struct nfs_removeres *res = task->tk_msg.rpc_resp;

	if (nfs4_async_handle_error(task, res->server, NULL) == -EAGAIN)
		return 0;
	update_changeattr(dir, &res->cinfo);
	nfs_post_op_update_inode(dir, &res->dir_attr);
	return 1;
}
