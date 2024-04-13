static int _nfs4_proc_lookupfh(struct nfs_server *server, const struct nfs_fh *dirfh,
		const struct qstr *name, struct nfs_fh *fhandle,
		struct nfs_fattr *fattr)
{
	int		       status;
	struct nfs4_lookup_arg args = {
		.bitmask = server->attr_bitmask,
		.dir_fh = dirfh,
		.name = name,
	};
	struct nfs4_lookup_res res = {
		.server = server,
		.fattr = fattr,
		.fh = fhandle,
	};
	struct rpc_message msg = {
		.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_LOOKUP],
		.rpc_argp = &args,
		.rpc_resp = &res,
	};

	nfs_fattr_init(fattr);

	dprintk("NFS call  lookupfh %s\n", name->name);
	status = rpc_call_sync(server->client, &msg, 0);
	dprintk("NFS reply lookupfh: %d\n", status);
	return status;
}
