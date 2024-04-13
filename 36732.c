static int encode_lookup(struct xdr_stream *xdr, const struct qstr *name)
{
	int len = name->len;
	__be32 *p;

	RESERVE_SPACE(8 + len);
	WRITE32(OP_LOOKUP);
	WRITE32(len);
	WRITEMEM(name->name, len);

 	return 0;
 }
