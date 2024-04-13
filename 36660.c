static int decode_attr_group(struct xdr_stream *xdr, uint32_t *bitmap, struct nfs_client *clp, uint32_t *gid)
{
	uint32_t len;
	__be32 *p;

	*gid = -2;
	if (unlikely(bitmap[1] & (FATTR4_WORD1_OWNER_GROUP - 1U)))
		return -EIO;
	if (likely(bitmap[1] & FATTR4_WORD1_OWNER_GROUP)) {
		READ_BUF(4);
		READ32(len);
		READ_BUF(len);
		if (len < XDR_MAX_NETOBJ) {
			if (nfs_map_group_to_gid(clp, (char *)p, len, gid) != 0)
				dprintk("%s: nfs_map_group_to_gid failed!\n",
						__func__);
		} else
			dprintk("%s: name too long (%u)!\n",
					__func__, len);
		bitmap[1] &= ~FATTR4_WORD1_OWNER_GROUP;
	}
	dprintk("%s: gid=%d\n", __func__, (int)*gid);
	return 0;
}
