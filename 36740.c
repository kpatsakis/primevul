static int encode_readlink(struct xdr_stream *xdr, const struct nfs4_readlink *readlink, struct rpc_rqst *req)
{
	__be32 *p;

	RESERVE_SPACE(4);
	WRITE32(OP_READLINK);

	return 0;
}
