static int decode_attr_owner(struct xdr_stream *xdr, uint32_t *bitmap, struct nfs_client *clp, uint32_t *uid)
{
	uint32_t len;
	__be32 *p;

	*uid = -2;
	if (unlikely(bitmap[1] & (FATTR4_WORD1_OWNER - 1U)))
		return -EIO;
	if (likely(bitmap[1] & FATTR4_WORD1_OWNER)) {
		READ_BUF(4);
		READ32(len);
		READ_BUF(len);
		if (len < XDR_MAX_NETOBJ) {
			if (nfs_map_name_to_uid(clp, (char *)p, len, uid) != 0)
				dprintk("%s: nfs_map_name_to_uid failed!\n",
						__func__);
		} else
			dprintk("%s: name too long (%u)!\n",
					__func__, len);
		bitmap[1] &= ~FATTR4_WORD1_OWNER;
	}
	dprintk("%s: uid=%d\n", __func__, (int)*uid);
	return 0;
}
