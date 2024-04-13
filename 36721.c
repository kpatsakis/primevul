static int encode_close(struct xdr_stream *xdr, const struct nfs_closeargs *arg)
{
	__be32 *p;

	RESERVE_SPACE(8+NFS4_STATEID_SIZE);
	WRITE32(OP_CLOSE);
	WRITE32(arg->seqid->sequence->counter);
	WRITEMEM(arg->stateid->data, NFS4_STATEID_SIZE);
	
	return 0;
}
