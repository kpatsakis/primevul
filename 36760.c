static int nfs4_xdr_dec_link(struct rpc_rqst *rqstp, __be32 *p, struct nfs4_link_res *res)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr;
	int status;
	
	xdr_init_decode(&xdr, &rqstp->rq_rcv_buf, p);
	if ((status = decode_compound_hdr(&xdr, &hdr)) != 0)
		goto out;
	if ((status = decode_putfh(&xdr)) != 0)
		goto out;
	if ((status = decode_savefh(&xdr)) != 0)
		goto out;
	if ((status = decode_putfh(&xdr)) != 0)
		goto out;
	if ((status = decode_link(&xdr, &res->cinfo)) != 0)
		goto out;
	/*
	 * Note order: OP_LINK leaves the directory as the current
	 *             filehandle.
	 */
	if (decode_getfattr(&xdr, res->dir_attr, res->server) != 0)
		goto out;
	if ((status = decode_restorefh(&xdr)) != 0)
		goto out;
	decode_getfattr(&xdr, res->fattr, res->server);
out:
	return status;
}
