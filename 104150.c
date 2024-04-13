static int is_valid_path(const char *path)
{
	const char *p = path;

	if (UNEXPECTED(!p)) {
		return 0;
	}
	if (UNEXPECTED(*p == '.') && *(p+1) == '.' && (!*(p+2) || IS_SLASH(*(p+2)))) {
		return 0;
	}
	while (*p) {
		if (IS_SLASH(*p)) {
			p++;
			if (UNEXPECTED(*p == '.')) {
				p++;
				if (UNEXPECTED(*p == '.')) {
					p++;
					if (UNEXPECTED(!*p) || UNEXPECTED(IS_SLASH(*p))) {
						return 0;
					}											
				}
			}
		}
		p++;
	}
	return 1;
}
