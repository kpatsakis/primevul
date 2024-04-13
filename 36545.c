static int _nfs4_proc_rename(struct inode *old_dir, struct qstr *old_name,
		struct inode *new_dir, struct qstr *new_name)
{
	struct nfs_server *server = NFS_SERVER(old_dir);
	struct nfs4_rename_arg arg = {
		.old_dir = NFS_FH(old_dir),
		.new_dir = NFS_FH(new_dir),
		.old_name = old_name,
		.new_name = new_name,
		.bitmask = server->attr_bitmask,
	};
	struct nfs_fattr old_fattr, new_fattr;
	struct nfs4_rename_res res = {
		.server = server,
		.old_fattr = &old_fattr,
		.new_fattr = &new_fattr,
	};
	struct rpc_message msg = {
		.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_RENAME],
		.rpc_argp = &arg,
		.rpc_resp = &res,
	};
	int			status;
	
	nfs_fattr_init(res.old_fattr);
	nfs_fattr_init(res.new_fattr);
	status = rpc_call_sync(server->client, &msg, 0);

	if (!status) {
		update_changeattr(old_dir, &res.old_cinfo);
		nfs_post_op_update_inode(old_dir, res.old_fattr);
		update_changeattr(new_dir, &res.new_cinfo);
		nfs_post_op_update_inode(new_dir, res.new_fattr);
	}
	return status;
}
