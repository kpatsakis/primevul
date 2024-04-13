encode_setacl(struct xdr_stream *xdr, struct nfs_setaclargs *arg)
{
	__be32 *p;

	RESERVE_SPACE(4+NFS4_STATEID_SIZE);
	WRITE32(OP_SETATTR);
	WRITEMEM(zero_stateid.data, NFS4_STATEID_SIZE);
	RESERVE_SPACE(2*4);
	WRITE32(1);
	WRITE32(FATTR4_WORD0_ACL);
	if (arg->acl_len % 4)
		return -EINVAL;
	RESERVE_SPACE(4);
	WRITE32(arg->acl_len);
	xdr_write_pages(xdr, arg->acl_pages, arg->acl_pgbase, arg->acl_len);
	return 0;
}
