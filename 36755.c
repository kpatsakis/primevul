static int nfs4_xdr_dec_delegreturn(struct rpc_rqst *rqstp, __be32 *p, struct nfs4_delegreturnres *res)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr;
	int status;

	xdr_init_decode(&xdr, &rqstp->rq_rcv_buf, p);
	status = decode_compound_hdr(&xdr, &hdr);
	if (status != 0)
		goto out;
	status = decode_putfh(&xdr);
	if (status != 0)
		goto out;
	status = decode_delegreturn(&xdr);
	decode_getfattr(&xdr, res->fattr, res->server);
out:
	return status;
}
