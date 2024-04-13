static int nfs4_xdr_dec_statfs(struct rpc_rqst *req, __be32 *p, struct nfs_fsstat *fsstat)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr;
	int status;

	xdr_init_decode(&xdr, &req->rq_rcv_buf, p);
	status = decode_compound_hdr(&xdr, &hdr);
	if (!status)
		status = decode_putfh(&xdr);
	if (!status)
		status = decode_statfs(&xdr, fsstat);
	return status;
}
