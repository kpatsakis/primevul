static int decode_op_hdr(struct xdr_stream *xdr, enum nfs_opnum4 expected)
{
	__be32 *p;
	uint32_t opnum;
	int32_t nfserr;

	READ_BUF(8);
	READ32(opnum);
	if (opnum != expected) {
		dprintk("nfs: Server returned operation"
			" %d but we issued a request for %d\n",
				opnum, expected);
		return -EIO;
	}
	READ32(nfserr);
	if (nfserr != NFS_OK)
		return nfs4_stat_to_errno(nfserr);
	return 0;
}
