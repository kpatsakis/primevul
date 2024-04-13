static int decode_change_info(struct xdr_stream *xdr, struct nfs4_change_info *cinfo)
{
	__be32 *p;

	READ_BUF(20);
	READ32(cinfo->atomic);
	READ64(cinfo->before);
	READ64(cinfo->after);
	return 0;
}
