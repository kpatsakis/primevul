static int decode_open_downgrade(struct xdr_stream *xdr, struct nfs_closeres *res)
{
	__be32 *p;
	int status;

	status = decode_op_hdr(xdr, OP_OPEN_DOWNGRADE);
	if (status != -EIO)
		nfs_increment_open_seqid(status, res->seqid);
	if (status)
		return status;
	READ_BUF(NFS4_STATEID_SIZE);
	COPYMEM(res->stateid.data, NFS4_STATEID_SIZE);
	return 0;
}
