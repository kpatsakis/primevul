static void encode_opentype(struct xdr_stream *xdr, const struct nfs_openargs *arg)
{
	__be32 *p;

	RESERVE_SPACE(4);
	switch (arg->open_flags & O_CREAT) {
		case 0:
			WRITE32(NFS4_OPEN_NOCREATE);
			break;
		default:
			BUG_ON(arg->claim != NFS4_OPEN_CLAIM_NULL);
			WRITE32(NFS4_OPEN_CREATE);
			encode_createmode(xdr, arg);
	}
}
