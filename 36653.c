static int decode_attr_bitmap(struct xdr_stream *xdr, uint32_t *bitmap)
{
	uint32_t bmlen;
	__be32 *p;

	READ_BUF(4);
	READ32(bmlen);

	bitmap[0] = bitmap[1] = 0;
	READ_BUF((bmlen << 2));
	if (bmlen > 0) {
		READ32(bitmap[0]);
		if (bmlen > 1)
			READ32(bitmap[1]);
	}
	return 0;
}
