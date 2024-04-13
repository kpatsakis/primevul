static int encode_link(struct xdr_stream *xdr, const struct qstr *name)
{
	__be32 *p;

	RESERVE_SPACE(8 + name->len);
	WRITE32(OP_LINK);
	WRITE32(name->len);
	WRITEMEM(name->name, name->len);
	
	return 0;
}
