static int nfs4_xdr_dec_setclientid_confirm(struct rpc_rqst *req, __be32 *p, struct nfs_fsinfo *fsinfo)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr;
	int status;

	xdr_init_decode(&xdr, &req->rq_rcv_buf, p);
	status = decode_compound_hdr(&xdr, &hdr);
	if (!status)
		status = decode_setclientid_confirm(&xdr);
	if (!status)
		status = decode_putrootfh(&xdr);
	if (!status)
		status = decode_fsinfo(&xdr, fsinfo);
	if (!status)
		status = nfs4_stat_to_errno(hdr.status);
	return status;
}
