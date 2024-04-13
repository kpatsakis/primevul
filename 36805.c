static int nfs4_xdr_enc_server_caps(struct rpc_rqst *req, __be32 *p, const struct nfs_fh *fhandle)
{
	struct xdr_stream xdr;
	struct compound_hdr hdr = {
		.nops = 2,
	};
	int status;

	xdr_init_encode(&xdr, &req->rq_snd_buf, p);
	encode_compound_hdr(&xdr, &hdr);
	status = encode_putfh(&xdr, fhandle);
	if (status == 0)
		status = encode_getattr_one(&xdr, FATTR4_WORD0_SUPPORTED_ATTRS|
				FATTR4_WORD0_LINK_SUPPORT|
				FATTR4_WORD0_SYMLINK_SUPPORT|
				FATTR4_WORD0_ACLSUPPORT);
	return status;
}
