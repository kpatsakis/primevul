static inline void encode_claim_null(struct xdr_stream *xdr, const struct qstr *name)
{
	__be32 *p;

	RESERVE_SPACE(4);
	WRITE32(NFS4_OPEN_CLAIM_NULL);
	encode_string(xdr, name->len, name->name);
}
