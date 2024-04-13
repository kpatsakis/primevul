static inline void encode_claim_delegate_cur(struct xdr_stream *xdr, const struct qstr *name, const nfs4_stateid *stateid)
{
	__be32 *p;

	RESERVE_SPACE(4+NFS4_STATEID_SIZE);
	WRITE32(NFS4_OPEN_CLAIM_DELEGATE_CUR);
	WRITEMEM(stateid->data, NFS4_STATEID_SIZE);
	encode_string(xdr, name->len, name->name);
}
