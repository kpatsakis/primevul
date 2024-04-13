static int decode_fsinfo(struct xdr_stream *xdr, struct nfs_fsinfo *fsinfo)
{
	__be32 *savep;
	uint32_t attrlen, bitmap[2];
	int status;

	if ((status = decode_op_hdr(xdr, OP_GETATTR)) != 0)
		goto xdr_error;
	if ((status = decode_attr_bitmap(xdr, bitmap)) != 0)
		goto xdr_error;
	if ((status = decode_attr_length(xdr, &attrlen, &savep)) != 0)
		goto xdr_error;

	fsinfo->rtmult = fsinfo->wtmult = 512;	/* ??? */

	if ((status = decode_attr_lease_time(xdr, bitmap, &fsinfo->lease_time)) != 0)
		goto xdr_error;
	if ((status = decode_attr_maxfilesize(xdr, bitmap, &fsinfo->maxfilesize)) != 0)
		goto xdr_error;
	if ((status = decode_attr_maxread(xdr, bitmap, &fsinfo->rtmax)) != 0)
		goto xdr_error;
	fsinfo->rtpref = fsinfo->dtpref = fsinfo->rtmax;
	if ((status = decode_attr_maxwrite(xdr, bitmap, &fsinfo->wtmax)) != 0)
		goto xdr_error;
	fsinfo->wtpref = fsinfo->wtmax;

	status = verify_attr_len(xdr, savep, attrlen);
xdr_error:
	dprintk("%s: xdr returned %d!\n", __func__, -status);
	return status;
}
