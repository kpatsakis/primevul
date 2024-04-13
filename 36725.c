static inline void encode_delegation_type(struct xdr_stream *xdr, fmode_t delegation_type)
{
	__be32 *p;

	RESERVE_SPACE(4);
	switch (delegation_type) {
		case 0:
			WRITE32(NFS4_OPEN_DELEGATE_NONE);
			break;
		case FMODE_READ:
			WRITE32(NFS4_OPEN_DELEGATE_READ);
			break;
		case FMODE_WRITE|FMODE_READ:
			WRITE32(NFS4_OPEN_DELEGATE_WRITE);
			break;
		default:
			BUG();
	}
}
