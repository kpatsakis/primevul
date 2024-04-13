static int encode_write(struct xdr_stream *xdr, const struct nfs_writeargs *args)
{
	__be32 *p;

	RESERVE_SPACE(4);
	WRITE32(OP_WRITE);

	encode_stateid(xdr, args->context);

	RESERVE_SPACE(16);
	WRITE64(args->offset);
	WRITE32(args->stable);
	WRITE32(args->count);

	xdr_write_pages(xdr, args->pages, args->pgbase, args->count);

	return 0;
}
