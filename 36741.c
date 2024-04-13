static int encode_rename(struct xdr_stream *xdr, const struct qstr *oldname, const struct qstr *newname)
{
	__be32 *p;

	RESERVE_SPACE(8 + oldname->len);
	WRITE32(OP_RENAME);
	WRITE32(oldname->len);
	WRITEMEM(oldname->name, oldname->len);
	
	RESERVE_SPACE(4 + newname->len);
	WRITE32(newname->len);
	WRITEMEM(newname->name, newname->len);

	return 0;
}
