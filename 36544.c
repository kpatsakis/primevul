static int _nfs4_proc_remove(struct inode *dir, struct qstr *name)
{
	struct nfs_server *server = NFS_SERVER(dir);
	struct nfs_removeargs args = {
		.fh = NFS_FH(dir),
		.name.len = name->len,
		.name.name = name->name,
		.bitmask = server->attr_bitmask,
	};
	struct nfs_removeres res = {
		.server = server,
	};
	struct rpc_message msg = {
		.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_REMOVE],
		.rpc_argp = &args,
		.rpc_resp = &res,
	};
	int			status;

	nfs_fattr_init(&res.dir_attr);
	status = rpc_call_sync(server->client, &msg, 0);
	if (status == 0) {
		update_changeattr(dir, &res.cinfo);
		nfs_post_op_update_inode(dir, &res.dir_attr);
	}
	return status;
}
