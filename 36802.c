static int nfs4_xdr_enc_readlink(struct rpc_rqst *req, __be32 *p, const struct nfs4_readlink *args)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr = {
		.nops = 2,
	};
	struct rpc_auth *auth = req->rq_task->tk_msg.rpc_cred->cr_auth;
	unsigned int replen;
	int status;

	xdr_init_encode(&xdr, &req->rq_snd_buf, p);
	encode_compound_hdr(&xdr, &hdr);
	status = encode_putfh(&xdr, args->fh);
	if(status)
		goto out;
	status = encode_readlink(&xdr, args, req);

	/* set up reply kvec
	 *    toplevel_status + taglen + rescount + OP_PUTFH + status
	 *      + OP_READLINK + status + string length = 8
	 */
	replen = (RPC_REPHDRSIZE + auth->au_rslack + NFS4_dec_readlink_sz) << 2;
	xdr_inline_pages(&req->rq_rcv_buf, replen, args->pages,
			args->pgbase, args->pglen);

out:
	return status;
}
