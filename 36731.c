static int encode_locku(struct xdr_stream *xdr, const struct nfs_locku_args *args)
{
	__be32 *p;

	RESERVE_SPACE(12+NFS4_STATEID_SIZE+16);
	WRITE32(OP_LOCKU);
	WRITE32(nfs4_lock_type(args->fl, 0));
	WRITE32(args->seqid->sequence->counter);
	WRITEMEM(args->stateid->data, NFS4_STATEID_SIZE);
	WRITE64(args->fl->fl_start);
	WRITE64(nfs4_lock_length(args->fl));

	return 0;
}
