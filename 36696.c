static int decode_locku(struct xdr_stream *xdr, struct nfs_locku_res *res)
{
	__be32 *p;
	int status;

	status = decode_op_hdr(xdr, OP_LOCKU);
	if (status != -EIO)
		nfs_increment_lock_seqid(status, res->seqid);
	if (status == 0) {
		READ_BUF(NFS4_STATEID_SIZE);
		COPYMEM(res->stateid.data, NFS4_STATEID_SIZE);
	}
	return status;
}
