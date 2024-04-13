static int ldm_parse_hexbyte (const u8 *src)
{
	unsigned int x;		/* For correct wrapping */
	int h;

	/* high part */
	x = h = hex_to_bin(src[0]);
	if (h < 0)
		return -1;

	/* low part */
	h = hex_to_bin(src[1]);
	if (h < 0)
		return -1;

	return (x << 4) + h;
}
