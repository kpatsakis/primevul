static int decode_ace(struct xdr_stream *xdr, void *ace, struct nfs_client *clp)
{
	__be32 *p;
	unsigned int strlen;
	char *str;

	READ_BUF(12);
	return decode_opaque_inline(xdr, &strlen, &str);
}
