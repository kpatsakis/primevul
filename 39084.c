void oz_cdev_rx(struct oz_pd *pd, struct oz_elt *elt)
{
	struct oz_serial_ctx *ctx;
	struct oz_app_hdr *app_hdr;
	u8 *data;
	int len;
	int space;
	int copy_sz;
	int ix;

	ctx = oz_cdev_claim_ctx(pd);
	if (ctx == NULL) {
		oz_dbg(ON, "Cannot claim serial context\n");
		return;
	}

	app_hdr = (struct oz_app_hdr *)(elt+1);
	/* If sequence number is non-zero then check it is not a duplicate.
	 */
	if (app_hdr->elt_seq_num != 0) {
		if (((ctx->rx_seq_num - app_hdr->elt_seq_num) & 0x80) == 0) {
			/* Reject duplicate element. */
			oz_dbg(ON, "Duplicate element:%02x %02x\n",
			       app_hdr->elt_seq_num, ctx->rx_seq_num);
			goto out;
		}
	}
	ctx->rx_seq_num = app_hdr->elt_seq_num;
	len = elt->length - sizeof(struct oz_app_hdr);
	data = ((u8 *)(elt+1)) + sizeof(struct oz_app_hdr);
	if (len <= 0)
		goto out;
	space = ctx->rd_out - ctx->rd_in - 1;
	if (space < 0)
		space += OZ_RD_BUF_SZ;
	if (len > space) {
		oz_dbg(ON, "Not enough space:%d %d\n", len, space);
		len = space;
	}
	ix = ctx->rd_in;
	copy_sz = OZ_RD_BUF_SZ - ix;
	if (copy_sz > len)
		copy_sz = len;
	memcpy(&ctx->rd_buf[ix], data, copy_sz);
	len -= copy_sz;
	ix += copy_sz;
	if (ix == OZ_RD_BUF_SZ)
		ix = 0;
	if (len) {
		memcpy(ctx->rd_buf, data+copy_sz, len);
		ix = len;
	}
	ctx->rd_in = ix;
	wake_up(&g_cdev.rdq);
out:
	oz_cdev_release_ctx(ctx);
}
