static int nfs4_xdr_dec_symlink(struct rpc_rqst *rqstp, __be32 *p, struct nfs4_create_res *res)
{
	return nfs4_xdr_dec_create(rqstp, p, res);
}
