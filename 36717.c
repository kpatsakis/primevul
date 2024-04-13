static int encode_access(struct xdr_stream *xdr, u32 access)
{
	__be32 *p;

	RESERVE_SPACE(8);
	WRITE32(OP_ACCESS);
	WRITE32(access);
	
	return 0;
}
