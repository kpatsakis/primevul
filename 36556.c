static int nfs4_do_create(struct inode *dir, struct dentry *dentry, struct nfs4_createdata *data)
{
	int status = rpc_call_sync(NFS_CLIENT(dir), &data->msg, 0);
	if (status == 0) {
		update_changeattr(dir, &data->res.dir_cinfo);
		nfs_post_op_update_inode(dir, data->res.dir_fattr);
		status = nfs_instantiate(dentry, data->res.fh, data->res.fattr);
	}
	return status;
}
