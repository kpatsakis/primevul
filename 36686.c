static int decode_commit(struct xdr_stream *xdr, struct nfs_writeres *res)
{
	__be32 *p;
	int status;

	status = decode_op_hdr(xdr, OP_COMMIT);
	if (status)
		return status;
	READ_BUF(8);
	COPYMEM(res->verf->verifier, 8);
	return 0;
}
