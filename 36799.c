static int nfs4_xdr_enc_pathconf(struct rpc_rqst *req, __be32 *p, const struct nfs4_pathconf_arg *args)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr = {
		.nops = 2,
	};
	int status;

	xdr_init_encode(&xdr, &req->rq_snd_buf, p);
	encode_compound_hdr(&xdr, &hdr);
	status = encode_putfh(&xdr, args->fh);
	if (!status)
		status = encode_getattr_one(&xdr,
				args->bitmask[0] & nfs4_pathconf_bitmap[0]);
	return status;
}
