static int decode_attr_space_free(struct xdr_stream *xdr, uint32_t *bitmap, uint64_t *res)
{
	__be32 *p;
	int status = 0;

	*res = 0;
	if (unlikely(bitmap[1] & (FATTR4_WORD1_SPACE_FREE - 1U)))
		return -EIO;
	if (likely(bitmap[1] & FATTR4_WORD1_SPACE_FREE)) {
		READ_BUF(8);
		READ64(*res);
		bitmap[1] &= ~FATTR4_WORD1_SPACE_FREE;
	}
	dprintk("%s: space free=%Lu\n", __func__, (unsigned long long)*res);
	return status;
}
