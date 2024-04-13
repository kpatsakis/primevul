static int nfs4_xdr_dec_fs_locations(struct rpc_rqst *req, __be32 *p, struct nfs4_fs_locations *res)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr;
	int status;

	xdr_init_decode(&xdr, &req->rq_rcv_buf, p);
	status = decode_compound_hdr(&xdr, &hdr);
	if (status != 0)
		goto out;
	if ((status = decode_putfh(&xdr)) != 0)
		goto out;
	if ((status = decode_lookup(&xdr)) != 0)
		goto out;
	xdr_enter_page(&xdr, PAGE_SIZE);
	status = decode_getfattr(&xdr, &res->fattr, res->server);
out:
	return status;
}
