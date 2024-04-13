encode_putfh(struct xdr_stream *xdr, const struct nfs_fh *fh)
{
	int len = fh->size;
	__be32 *p;

	RESERVE_SPACE(8 + len);
	WRITE32(OP_PUTFH);
	WRITE32(len);
	WRITEMEM(fh->data, len);

	return 0;
}
