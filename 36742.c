static int encode_renew(struct xdr_stream *xdr, const struct nfs_client *client_stateid)
{
	__be32 *p;

	RESERVE_SPACE(12);
	WRITE32(OP_RENEW);
	WRITE64(client_stateid->cl_clientid);

	return 0;
}
