int http_transform_header_str(struct stream* s, struct http_msg *msg,
                              const char* name, unsigned int name_len,
                              const char *str, struct my_regex *re,
                              int action)
{
	struct hdr_ctx ctx;
	char *buf = msg->chn->buf->p;
	struct hdr_idx *idx = &s->txn->hdr_idx;
	int (*http_find_hdr_func)(const char *name, int len, char *sol,
	                          struct hdr_idx *idx, struct hdr_ctx *ctx);
	struct chunk *output = get_trash_chunk();

	ctx.idx = 0;

	/* Choose the header browsing function. */
	switch (action) {
	case ACT_HTTP_REPLACE_VAL:
		http_find_hdr_func = http_find_header2;
		break;
	case ACT_HTTP_REPLACE_HDR:
		http_find_hdr_func = http_find_full_header2;
		break;
	default: /* impossible */
		return -1;
	}

	while (http_find_hdr_func(name, name_len, buf, idx, &ctx)) {
		struct hdr_idx_elem *hdr = idx->v + ctx.idx;
		int delta;
		char *val = ctx.line + ctx.val;
		char* val_end = val + ctx.vlen;

		if (!regex_exec_match2(re, val, val_end-val, MAX_MATCH, pmatch, 0))
			continue;

		output->len = exp_replace(output->str, output->size, val, str, pmatch);
		if (output->len == -1)
			return -1;

		delta = buffer_replace2(msg->chn->buf, val, val_end, output->str, output->len);

		hdr->len += delta;
		http_msg_move_end(msg, delta);

		/* Adjust the length of the current value of the index. */
		ctx.vlen += delta;
	}

	return 0;
}
