find_url_param_pos(const char **chunks,
                   const char* url_param_name, size_t url_param_name_l,
                   char delim)
{
	const char *pos, *last, *equal;
	const char **bufs = chunks;
	int l1, l2;


	pos  = bufs[0];
	last = bufs[1];
	while (pos < last) {
		/* Check the equal. */
		equal = pos + url_param_name_l;
		if (fix_pointer_if_wrap(chunks, &equal)) {
			if (equal >= chunks[3])
				return NULL;
		} else {
			if (equal >= chunks[1])
				return NULL;
		}
		if (*equal == '=') {
			if (pos + url_param_name_l > last) {
				/* process wrap case, we detect a wrap. In this case, the
				 * comparison is performed in two parts.
				 */

				/* This is the end, we dont have any other chunk. */
				if (bufs != chunks || !bufs[2])
					return NULL;

				/* Compute the length of each part of the comparison. */
				l1 = last - pos;
				l2 = url_param_name_l - l1;

				/* The second buffer is too short to contain the compared string. */
				if (bufs[2] + l2 > bufs[3])
					return NULL;

				if (memcmp(pos,     url_param_name,    l1) == 0 &&
				    memcmp(bufs[2], url_param_name+l1, l2) == 0)
					return pos;

				/* Perform wrapping and jump the string who fail the comparison. */
				bufs += 2;
				pos = bufs[0] + l2;
				last = bufs[1];

			} else {
				/* process a simple comparison. */
				if (memcmp(pos, url_param_name, url_param_name_l) == 0)
					return pos;
				pos += url_param_name_l + 1;
				if (fix_pointer_if_wrap(chunks, &pos))
					last = bufs[2];
			}
		}

		while (1) {
			/* Look for the next delimiter. */
			while (pos < last && !is_param_delimiter(*pos, delim))
				pos++;
			if (pos < last)
				break;
			/* process buffer wrapping. */
			if (bufs != chunks || !bufs[2])
				return NULL;
			bufs += 2;
			pos = bufs[0];
			last = bufs[1];
		}
		pos++;
	}
	return NULL;
}
