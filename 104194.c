file_pstring_get_length(const struct magic *m, const char *s)
{
	size_t len = 0;

	switch (m->str_flags & PSTRING_LEN) {
	case PSTRING_1_LE:
		len = *s;
		break;
	case PSTRING_2_LE:
		len = (s[1] << 8) | s[0];
		break;
	case PSTRING_2_BE:
		len = (s[0] << 8) | s[1];
		break;
	case PSTRING_4_LE:
		len = (s[3] << 24) | (s[2] << 16) | (s[1] << 8) | s[0];
		break;
	case PSTRING_4_BE:
		len = (s[0] << 24) | (s[1] << 16) | (s[2] << 8) | s[3];
		break;
	default:
		abort();	/* Impossible */
	}

	if (m->str_flags & PSTRING_LENGTH_INCLUDES_ITSELF)
		len -= file_pstring_length_size(m);

	return len;
}
