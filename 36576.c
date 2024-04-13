int nfs4_proc_fs_locations(struct inode *dir, const struct qstr *name,
		struct nfs4_fs_locations *fs_locations, struct page *page)
{
	struct nfs_server *server = NFS_SERVER(dir);
	u32 bitmask[2] = {
		[0] = FATTR4_WORD0_FSID | FATTR4_WORD0_FS_LOCATIONS,
		[1] = FATTR4_WORD1_MOUNTED_ON_FILEID,
	};
	struct nfs4_fs_locations_arg args = {
		.dir_fh = NFS_FH(dir),
		.name = name,
		.page = page,
		.bitmask = bitmask,
	};
	struct rpc_message msg = {
		.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_FS_LOCATIONS],
		.rpc_argp = &args,
		.rpc_resp = fs_locations,
	};
	int status;

	dprintk("%s: start\n", __func__);
	nfs_fattr_init(&fs_locations->fattr);
	fs_locations->server = server;
	fs_locations->nlocations = 0;
	status = rpc_call_sync(server->client, &msg, 0);
	dprintk("%s: returned status = %d\n", __func__, status);
	return status;
}
