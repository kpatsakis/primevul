static inline int fix_pointer_if_wrap(const char **chunks, const char **ptr)
{
	if (*ptr < chunks[1])
		return 0;
	if (!chunks[2])
		return 0;
	*ptr = chunks[2] + ( *ptr - chunks[1] );
	return 1;
}
