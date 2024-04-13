static int decode_getfh(struct xdr_stream *xdr, struct nfs_fh *fh)
{
	__be32 *p;
	uint32_t len;
	int status;

	/* Zero handle first to allow comparisons */
	memset(fh, 0, sizeof(*fh));

	status = decode_op_hdr(xdr, OP_GETFH);
	if (status)
		return status;

	READ_BUF(4);
	READ32(len);
	if (len > NFS4_FHSIZE)
		return -EIO;
	fh->size = len;
	READ_BUF(len);
	COPYMEM(fh->data, len);
	return 0;
}
