static int decode_open(struct xdr_stream *xdr, struct nfs_openres *res)
{
        __be32 *p;
	uint32_t savewords, bmlen, i;
        int status;

        status = decode_op_hdr(xdr, OP_OPEN);
	if (status != -EIO)
		nfs_increment_open_seqid(status, res->seqid);
        if (status)
                return status;
        READ_BUF(NFS4_STATEID_SIZE);
        COPYMEM(res->stateid.data, NFS4_STATEID_SIZE);

        decode_change_info(xdr, &res->cinfo);

        READ_BUF(8);
        READ32(res->rflags);
        READ32(bmlen);
        if (bmlen > 10)
                goto xdr_error;

        READ_BUF(bmlen << 2);
	savewords = min_t(uint32_t, bmlen, NFS4_BITMAP_SIZE);
	for (i = 0; i < savewords; ++i)
		READ32(res->attrset[i]);
	for (; i < NFS4_BITMAP_SIZE; i++)
		res->attrset[i] = 0;

	return decode_delegation(xdr, res);
xdr_error:
	dprintk("%s: Bitmap too large! Length = %u\n", __func__, bmlen);
	return -EIO;
}
