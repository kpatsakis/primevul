static int nfs4_xdr_enc_lookup_root(struct rpc_rqst *req, __be32 *p, const struct nfs4_lookup_root_arg *args)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr = {
		.nops = 3,
	};
	int status;

	xdr_init_encode(&xdr, &req->rq_snd_buf, p);
	encode_compound_hdr(&xdr, &hdr);
	if ((status = encode_putrootfh(&xdr)) != 0)
		goto out;
	if ((status = encode_getfh(&xdr)) == 0)
		status = encode_getfattr(&xdr, args->bitmask);
out:
	return status;
}
