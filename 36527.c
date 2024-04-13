static int __nfs4_proc_set_acl(struct inode *inode, const void *buf, size_t buflen)
{
	struct nfs_server *server = NFS_SERVER(inode);
	struct page *pages[NFS4ACL_MAXPAGES];
	struct nfs_setaclargs arg = {
		.fh		= NFS_FH(inode),
		.acl_pages	= pages,
		.acl_len	= buflen,
	};
	struct rpc_message msg = {
		.rpc_proc	= &nfs4_procedures[NFSPROC4_CLNT_SETACL],
		.rpc_argp	= &arg,
		.rpc_resp	= NULL,
	};
	int ret;

	if (!nfs4_server_supports_acls(server))
		return -EOPNOTSUPP;
	nfs_inode_return_delegation(inode);
	buf_to_pages(buf, buflen, arg.acl_pages, &arg.acl_pgbase);
	ret = rpc_call_sync(NFS_CLIENT(inode), &msg, 0);
	nfs_access_zap_cache(inode);
	nfs_zap_acl_cache(inode);
	return ret;
}
