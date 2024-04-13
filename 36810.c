static int nfs4_xdr_enc_statfs(struct rpc_rqst *req, __be32 *p, const struct nfs4_statfs_arg *args)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr = {
		.nops = 2,
	};
	int status;

	xdr_init_encode(&xdr, &req->rq_snd_buf, p);
	encode_compound_hdr(&xdr, &hdr);
	status = encode_putfh(&xdr, args->fh);
	if (status == 0)
		status = encode_getattr_two(&xdr,
				args->bitmask[0] & nfs4_statfs_bitmap[0],
				args->bitmask[1] & nfs4_statfs_bitmap[1]);
	return status;
}
