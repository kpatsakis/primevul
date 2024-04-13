static ssize_t __nfs4_get_acl_uncached(struct inode *inode, void *buf, size_t buflen)
{
	struct page *pages[NFS4ACL_MAXPAGES];
	struct nfs_getaclargs args = {
		.fh = NFS_FH(inode),
		.acl_pages = pages,
		.acl_len = buflen,
	};
	size_t resp_len = buflen;
	void *resp_buf;
	struct rpc_message msg = {
		.rpc_proc = &nfs4_procedures[NFSPROC4_CLNT_GETACL],
		.rpc_argp = &args,
		.rpc_resp = &resp_len,
	};
	struct page *localpage = NULL;
	int ret;

	if (buflen < PAGE_SIZE) {
		/* As long as we're doing a round trip to the server anyway,
		 * let's be prepared for a page of acl data. */
		localpage = alloc_page(GFP_KERNEL);
		resp_buf = page_address(localpage);
		if (localpage == NULL)
			return -ENOMEM;
		args.acl_pages[0] = localpage;
		args.acl_pgbase = 0;
		resp_len = args.acl_len = PAGE_SIZE;
	} else {
		resp_buf = buf;
		buf_to_pages(buf, buflen, args.acl_pages, &args.acl_pgbase);
	}
	ret = rpc_call_sync(NFS_CLIENT(inode), &msg, 0);
	if (ret)
		goto out_free;
	if (resp_len > args.acl_len)
		nfs4_write_cached_acl(inode, NULL, resp_len);
	else
		nfs4_write_cached_acl(inode, resp_buf, resp_len);
	if (buf) {
		ret = -ERANGE;
		if (resp_len > buflen)
			goto out_free;
		if (localpage)
			memcpy(buf, resp_buf, resp_len);
	}
	ret = resp_len;
out_free:
	if (localpage)
		__free_page(localpage);
	return ret;
}
