static int decode_attr_nlink(struct xdr_stream *xdr, uint32_t *bitmap, uint32_t *nlink)
{
	__be32 *p;

	*nlink = 1;
	if (unlikely(bitmap[1] & (FATTR4_WORD1_NUMLINKS - 1U)))
		return -EIO;
	if (likely(bitmap[1] & FATTR4_WORD1_NUMLINKS)) {
		READ_BUF(4);
		READ32(*nlink);
		bitmap[1] &= ~FATTR4_WORD1_NUMLINKS;
	}
	dprintk("%s: nlink=%u\n", __func__, (unsigned int)*nlink);
	return 0;
}
