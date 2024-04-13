int del_hdr_value(struct buffer *buf, char **from, char *next)
{
	char *prev = *from;

	if (*prev == ':') {
		/* We're removing the first value, preserve the colon and add a
		 * space if possible.
		 */
		if (!HTTP_IS_CRLF(*next))
			next++;
		prev++;
		if (prev < next)
			*prev++ = ' ';

		while (HTTP_IS_SPHT(*next))
			next++;
	} else {
		/* Remove useless spaces before the old delimiter. */
		while (HTTP_IS_SPHT(*(prev-1)))
			prev--;
		*from = prev;

		/* copy the delimiter and if possible a space if we're
		 * not at the end of the line.
		 */
		if (!HTTP_IS_CRLF(*next)) {
			*prev++ = *next++;
			if (prev + 1 < next)
				*prev++ = ' ';
			while (HTTP_IS_SPHT(*next))
				next++;
		}
	}
	return buffer_replace2(buf, prev, next, NULL, 0);
}
