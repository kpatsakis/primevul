static int nfs4_xdr_dec_lookup_root(struct rpc_rqst *rqstp, __be32 *p, struct nfs4_lookup_res *res)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr;
	int status;
	
	xdr_init_decode(&xdr, &rqstp->rq_rcv_buf, p);
	if ((status = decode_compound_hdr(&xdr, &hdr)) != 0)
		goto out;
	if ((status = decode_putrootfh(&xdr)) != 0)
		goto out;
	if ((status = decode_getfh(&xdr, res->fh)) == 0)
		status = decode_getfattr(&xdr, res->fattr, res->server);
out:
	return status;
}
