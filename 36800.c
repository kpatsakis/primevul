static int nfs4_xdr_enc_read(struct rpc_rqst *req, __be32 *p, struct nfs_readargs *args)
{
	struct rpc_auth *auth = req->rq_task->tk_msg.rpc_cred->cr_auth;
	struct xdr_stream xdr;
	struct compound_hdr hdr = {
		.nops = 2,
	};
	int replen, status;

	xdr_init_encode(&xdr, &req->rq_snd_buf, p);
	encode_compound_hdr(&xdr, &hdr);
	status = encode_putfh(&xdr, args->fh);
	if (status)
		goto out;
	status = encode_read(&xdr, args);
	if (status)
		goto out;

	/* set up reply kvec
	 *    toplevel status + taglen=0 + rescount + OP_PUTFH + status
	 *       + OP_READ + status + eof + datalen = 9
	 */
	replen = (RPC_REPHDRSIZE + auth->au_rslack + NFS4_dec_read_sz) << 2;
	xdr_inline_pages(&req->rq_rcv_buf, replen,
			 args->pages, args->pgbase, args->count);
	req->rq_rcv_buf.flags |= XDRBUF_READ;
out:
	return status;
}
