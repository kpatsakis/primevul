nfs4_xdr_enc_getacl(struct rpc_rqst *req, __be32 *p,
		struct nfs_getaclargs *args)
{
	struct xdr_stream xdr;
	struct rpc_auth *auth = req->rq_task->tk_msg.rpc_cred->cr_auth;
	struct compound_hdr hdr = {
		.nops   = 2,
	};
	int replen, status;

	xdr_init_encode(&xdr, &req->rq_snd_buf, p);
	encode_compound_hdr(&xdr, &hdr);
	status = encode_putfh(&xdr, args->fh);
	if (status)
		goto out;
	status = encode_getattr_two(&xdr, FATTR4_WORD0_ACL, 0);
	/* set up reply buffer: */
	replen = (RPC_REPHDRSIZE + auth->au_rslack + NFS4_dec_getacl_sz) << 2;
	xdr_inline_pages(&req->rq_rcv_buf, replen,
		args->acl_pages, args->acl_pgbase, args->acl_len);
out:
	return status;
}
