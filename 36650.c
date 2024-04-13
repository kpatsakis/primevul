static int decode_access(struct xdr_stream *xdr, struct nfs4_accessres *access)
{
	__be32 *p;
	uint32_t supp, acc;
	int status;

	status = decode_op_hdr(xdr, OP_ACCESS);
	if (status)
		return status;
	READ_BUF(8);
	READ32(supp);
	READ32(acc);
	access->supported = supp;
	access->access = acc;
	return 0;
}
