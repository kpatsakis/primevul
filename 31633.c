static inline int language_range_match(const char *range, int range_len,
                                       const char *tag, int tag_len)
{
	const char *end = range + range_len;
	const char *tend = tag + tag_len;
	while (range < end) {
		if (*range == '-' && tag == tend)
			return 1;
		if (*range != *tag || tag == tend)
			return 0;
		range++;
		tag++;
	}
	/* Return true only if the last char of the tag is matched. */
	return tag == tend;
}
