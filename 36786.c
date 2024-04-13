static int nfs4_xdr_enc_delegreturn(struct rpc_rqst *req, __be32 *p, const struct nfs4_delegreturnargs *args)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr = {
		.nops = 3,
	};
	int status;

	xdr_init_encode(&xdr, &req->rq_snd_buf, p);
	encode_compound_hdr(&xdr, &hdr);
	status = encode_putfh(&xdr, args->fhandle);
	if (status != 0)
		goto out;
	status = encode_delegreturn(&xdr, args->stateid);
	if (status != 0)
		goto out;
	status = encode_getfattr(&xdr, args->bitmask);
out:
	return status;
}
