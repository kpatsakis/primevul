static int decode_setattr(struct xdr_stream *xdr, struct nfs_setattrres *res)
{
	__be32 *p;
	uint32_t bmlen;
	int status;

        
	status = decode_op_hdr(xdr, OP_SETATTR);
	if (status)
		return status;
	READ_BUF(4);
	READ32(bmlen);
	READ_BUF(bmlen << 2);
	return 0;
}
