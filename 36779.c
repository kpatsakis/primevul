static int nfs4_xdr_dec_setclientid(struct rpc_rqst *req, __be32 *p,
		struct nfs_client *clp)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr;
	int status;

	xdr_init_decode(&xdr, &req->rq_rcv_buf, p);
	status = decode_compound_hdr(&xdr, &hdr);
	if (!status)
		status = decode_setclientid(&xdr, clp);
	if (!status)
		status = nfs4_stat_to_errno(hdr.status);
	return status;
}
