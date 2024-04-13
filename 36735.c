static int encode_open_confirm(struct xdr_stream *xdr, const struct nfs_open_confirmargs *arg)
{
	__be32 *p;

	RESERVE_SPACE(4+NFS4_STATEID_SIZE+4);
	WRITE32(OP_OPEN_CONFIRM);
	WRITEMEM(arg->stateid->data, NFS4_STATEID_SIZE);
	WRITE32(arg->seqid->sequence->counter);

	return 0;
}
