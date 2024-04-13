static int http_transform_header(struct session* s, struct http_msg *msg, const char* name, uint name_len,
                                 char* buf, struct hdr_idx* idx, struct list *fmt, struct my_regex *re,
                                 struct hdr_ctx* ctx, int action)
{
	ctx->idx = 0;

	while (http_find_full_header2(name, name_len, buf, idx, ctx)) {
		struct hdr_idx_elem *hdr = idx->v + ctx->idx;
		int delta;
		char* val = (char*)ctx->line + name_len + 2;
		char* val_end = (char*)ctx->line + hdr->len;
		char* reg_dst_buf;
		uint reg_dst_buf_size;
		int n_replaced;

		trash.len = build_logline(s, trash.str, trash.size, fmt);

		if (trash.len >= trash.size - 1)
			return -1;

		reg_dst_buf = trash.str + trash.len + 1;
		reg_dst_buf_size = trash.size - trash.len - 1;

		switch (action) {
		case HTTP_REQ_ACT_REPLACE_VAL:
		case HTTP_RES_ACT_REPLACE_VAL:
			n_replaced = http_replace_value(re, reg_dst_buf, reg_dst_buf_size, val, val_end-val, ',', trash.str);
			break;
		case HTTP_REQ_ACT_REPLACE_HDR:
		case HTTP_RES_ACT_REPLACE_HDR:
			n_replaced = http_replace_header(re, reg_dst_buf, reg_dst_buf_size, val, val_end-val, trash.str);
			break;
		default: /* impossible */
			return -1;
		}

		switch (n_replaced) {
		case -1: return -1;
		case -2: continue;
		}

		delta = buffer_replace2(msg->chn->buf, val, val_end, reg_dst_buf, n_replaced);

		hdr->len += delta;
		http_msg_move_end(msg, delta);
	}

	return 0;
}
