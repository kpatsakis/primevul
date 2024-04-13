static inline int decode_attr_length(struct xdr_stream *xdr, uint32_t *attrlen, __be32 **savep)
{
	__be32 *p;

	READ_BUF(4);
	READ32(*attrlen);
	*savep = xdr->p;
	return 0;
}
