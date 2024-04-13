static int http_transform_header(struct stream* s, struct http_msg *msg,
                                 const char* name, unsigned int name_len,
                                 struct list *fmt, struct my_regex *re,
                                 int action)
{
	struct chunk *replace;
	int ret = -1;

	replace = alloc_trash_chunk();
	if (!replace)
		goto leave;

	replace->len = build_logline(s, replace->str, replace->size, fmt);
	if (replace->len >= replace->size - 1)
		goto leave;

	ret = http_transform_header_str(s, msg, name, name_len, replace->str, re, action);

  leave:
	free_trash_chunk(replace);
	return ret;
}
