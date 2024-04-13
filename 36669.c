static int decode_attr_mode(struct xdr_stream *xdr, uint32_t *bitmap, uint32_t *mode)
{
	__be32 *p;

	*mode = 0;
	if (unlikely(bitmap[1] & (FATTR4_WORD1_MODE - 1U)))
		return -EIO;
	if (likely(bitmap[1] & FATTR4_WORD1_MODE)) {
		READ_BUF(4);
		READ32(*mode);
		*mode &= ~S_IFMT;
		bitmap[1] &= ~FATTR4_WORD1_MODE;
	}
	dprintk("%s: file mode=0%o\n", __func__, (unsigned int)*mode);
	return 0;
}
