int buffer_replace2(struct buffer *b, char *pos, char *end, const char *str, int len)
{
	int delta;

	delta = len - (end - pos);

	if (bi_end(b) + delta > b->data + b->size)
		return 0;  /* no space left */

	if (buffer_not_empty(b) &&
	    bi_end(b) + delta > bo_ptr(b) &&
	    bo_ptr(b) >= bi_end(b))
		return 0;  /* no space left before wrapping data */

	/* first, protect the end of the buffer */
	memmove(end + delta, end, bi_end(b) - end);

	/* now, copy str over pos */
	if (len)
		memcpy(pos, str, len);

	b->i += delta;

	if (buffer_empty(b))
		b->p = b->data;

	return delta;
}
