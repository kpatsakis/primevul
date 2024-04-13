static int nfs4_xdr_enc_write(struct rpc_rqst *req, __be32 *p, struct nfs_writeargs *args)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr = {
		.nops = 3,
	};
	int status;

	xdr_init_encode(&xdr, &req->rq_snd_buf, p);
	encode_compound_hdr(&xdr, &hdr);
	status = encode_putfh(&xdr, args->fh);
	if (status)
		goto out;
	status = encode_write(&xdr, args);
	if (status)
		goto out;
	req->rq_snd_buf.flags |= XDRBUF_WRITE;
	status = encode_getfattr(&xdr, args->bitmask);
out:
	return status;
}
