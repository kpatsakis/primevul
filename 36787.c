static int nfs4_xdr_enc_fs_locations(struct rpc_rqst *req, __be32 *p, struct nfs4_fs_locations_arg *args)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr = {
		.nops = 3,
	};
	struct rpc_auth *auth = req->rq_task->tk_msg.rpc_cred->cr_auth;
	int replen;
	int status;

	xdr_init_encode(&xdr, &req->rq_snd_buf, p);
	encode_compound_hdr(&xdr, &hdr);
	if ((status = encode_putfh(&xdr, args->dir_fh)) != 0)
		goto out;
	if ((status = encode_lookup(&xdr, args->name)) != 0)
		goto out;
	if ((status = encode_fs_locations(&xdr, args->bitmask)) != 0)
		goto out;
	/* set up reply
	 *   toplevel_status + OP_PUTFH + status
	 *   + OP_LOOKUP + status + OP_GETATTR + status = 7
	 */
	replen = (RPC_REPHDRSIZE + auth->au_rslack + 7) << 2;
	xdr_inline_pages(&req->rq_rcv_buf, replen, &args->page,
			0, PAGE_SIZE);
out:
	return status;
}
