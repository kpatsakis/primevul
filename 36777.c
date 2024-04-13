nfs4_xdr_dec_setacl(struct rpc_rqst *rqstp, __be32 *p, void *res)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr;
	int status;

	xdr_init_decode(&xdr, &rqstp->rq_rcv_buf, p);
	status = decode_compound_hdr(&xdr, &hdr);
	if (status)
		goto out;
	status = decode_putfh(&xdr);
	if (status)
		goto out;
	status = decode_setattr(&xdr, res);
out:
	return status;
}
