encode_savefh(struct xdr_stream *xdr)
{
	__be32 *p;

	RESERVE_SPACE(4);
	WRITE32(OP_SAVEFH);

	return 0;
}
