static int ldm_relative(const u8 *buffer, int buflen, int base, int offset)
{

	base += offset;
	if (!buffer || offset < 0 || base > buflen) {
		if (!buffer)
			ldm_error("!buffer");
		if (offset < 0)
			ldm_error("offset (%d) < 0", offset);
		if (base > buflen)
			ldm_error("base (%d) > buflen (%d)", base, buflen);
		return -1;
	}
	if (base + buffer[base] >= buflen) {
		ldm_error("base (%d) + buffer[base] (%d) >= buflen (%d)", base,
				buffer[base], buflen);
		return -1;
	}
	return buffer[base] + offset + 1;
}
