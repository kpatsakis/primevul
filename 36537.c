static int _nfs4_proc_link(struct inode *inode, struct inode *dir, struct qstr *name)
{
	struct nfs_server *server = NFS_SERVER(inode);
	struct nfs4_link_arg arg = {
		.fh     = NFS_FH(inode),
		.dir_fh = NFS_FH(dir),
		.name   = name,
		.bitmask = server->attr_bitmask,
	};
	struct nfs_fattr fattr, dir_attr;
	struct nfs4_link_res res = {
		.server = server,
		.fattr = &fattr,
		.dir_attr = &dir_attr,
	};
	struct rpc_message msg = {
		.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_LINK],
		.rpc_argp = &arg,
		.rpc_resp = &res,
	};
	int			status;

	nfs_fattr_init(res.fattr);
	nfs_fattr_init(res.dir_attr);
	status = rpc_call_sync(server->client, &msg, 0);
	if (!status) {
		update_changeattr(dir, &res.cinfo);
		nfs_post_op_update_inode(dir, res.dir_attr);
		nfs_post_op_update_inode(inode, res.fattr);
	}

	return status;
}
