static int decode_attr_space_avail(struct xdr_stream *xdr, uint32_t *bitmap, uint64_t *res)
{
	__be32 *p;
	int status = 0;

	*res = 0;
	if (unlikely(bitmap[1] & (FATTR4_WORD1_SPACE_AVAIL - 1U)))
		return -EIO;
	if (likely(bitmap[1] & FATTR4_WORD1_SPACE_AVAIL)) {
		READ_BUF(8);
		READ64(*res);
		bitmap[1] &= ~FATTR4_WORD1_SPACE_AVAIL;
	}
	dprintk("%s: space avail=%Lu\n", __func__, (unsigned long long)*res);
	return status;
}
