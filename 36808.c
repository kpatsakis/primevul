static int nfs4_xdr_enc_setclientid(struct rpc_rqst *req, __be32 *p, struct nfs4_setclientid *sc)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr = {
		.nops	= 1,
	};

	xdr_init_encode(&xdr, &req->rq_snd_buf, p);
	encode_compound_hdr(&xdr, &hdr);
	return encode_setclientid(&xdr, sc);
}
