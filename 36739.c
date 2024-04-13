static int encode_read(struct xdr_stream *xdr, const struct nfs_readargs *args)
{
	__be32 *p;

	RESERVE_SPACE(4);
	WRITE32(OP_READ);

	encode_stateid(xdr, args->context);

	RESERVE_SPACE(12);
	WRITE64(args->offset);
	WRITE32(args->count);

	return 0;
}
