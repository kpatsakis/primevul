void nfs4_reset_write(struct rpc_task *task, struct nfs_write_data *data)
{
	dprintk("%s Reset task for i/o through\n", __func__);
	put_lseg(data->lseg);
	data->lseg          = NULL;
	data->ds_clp        = NULL;
	data->write_done_cb = nfs4_write_done_cb;
	data->args.fh       = NFS_FH(data->inode);
	data->args.bitmask  = data->res.server->cache_consistency_bitmask;
	data->args.offset   = data->mds_offset;
	data->res.fattr     = &data->fattr;
	task->tk_ops        = data->mds_ops;
	rpc_task_reset_client(task, NFS_CLIENT(data->inode));
}
