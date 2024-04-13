static inline void dump_hex(FILE *ofd, char *start, int len, int prefix)
{
	int i, a, c, limit;
	char ascii[21] = {0};

	limit = 16 - (prefix * 2);
	prefix_line(ofd, prefix);
	fprintf(ofd, "    ");

	for (i = 0, a = 0, c = 0; i < len; i++) {
		int v = *(uint8_t *) (start + i);

		fprintf(ofd, "%02x ", v);
		ascii[a++] = isprint(v) ? v : '.';

		if (++c >= limit) {
			fprintf(ofd, "%s\n", ascii);
			if (i < (len - 1)) {
				prefix_line(ofd, prefix);
				fprintf(ofd, "    ");
			}
			a = c = 0;
			memset(ascii, 0, sizeof(ascii));
		}
	}

	if (c != 0) {
		for (i = 0; i < (limit - c); i++)
			fprintf(ofd, "   ");
		fprintf(ofd, "%s\n", ascii);
	}
}
