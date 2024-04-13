static int decode_opaque_inline(struct xdr_stream *xdr, unsigned int *len, char **string)
{
	__be32 *p;

	READ_BUF(4);
	READ32(*len);
	READ_BUF(*len);
	*string = (char *)p;
	return 0;
}
