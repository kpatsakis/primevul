static int encode_lockt(struct xdr_stream *xdr, const struct nfs_lockt_args *args)
{
	__be32 *p;

	RESERVE_SPACE(52);
	WRITE32(OP_LOCKT);
	WRITE32(nfs4_lock_type(args->fl, 0));
	WRITE64(args->fl->fl_start);
	WRITE64(nfs4_lock_length(args->fl));
	WRITE64(args->lock_owner.clientid);
	WRITE32(16);
	WRITEMEM("lock id:", 8);
	WRITE64(args->lock_owner.id);

	return 0;
}
