static inline void encode_createmode(struct xdr_stream *xdr, const struct nfs_openargs *arg)
{
	__be32 *p;

	RESERVE_SPACE(4);
	switch(arg->open_flags & O_EXCL) {
		case 0:
			WRITE32(NFS4_CREATE_UNCHECKED);
			encode_attrs(xdr, arg->u.attrs, arg->server);
			break;
		default:
			WRITE32(NFS4_CREATE_EXCLUSIVE);
			encode_nfs4_verifier(xdr, &arg->u.verifier);
	}
}
