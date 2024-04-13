static int nfs4_xdr_enc_readdir(struct rpc_rqst *req, __be32 *p, const struct nfs4_readdir_arg *args)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr = {
		.nops = 2,
	};
	struct rpc_auth *auth = req->rq_task->tk_msg.rpc_cred->cr_auth;
	int replen;
	int status;

	xdr_init_encode(&xdr, &req->rq_snd_buf, p);
	encode_compound_hdr(&xdr, &hdr);
	status = encode_putfh(&xdr, args->fh);
	if(status)
		goto out;
	status = encode_readdir(&xdr, args, req);

	/* set up reply kvec
	 *    toplevel_status + taglen + rescount + OP_PUTFH + status
	 *      + OP_READDIR + status + verifer(2)  = 9
	 */
	replen = (RPC_REPHDRSIZE + auth->au_rslack + NFS4_dec_readdir_sz) << 2;
	xdr_inline_pages(&req->rq_rcv_buf, replen, args->pages,
			 args->pgbase, args->count);
	dprintk("%s: inlined page args = (%u, %p, %u, %u)\n",
			__func__, replen, args->pages,
			args->pgbase, args->count);

out:
	return status;
}
