static int ldm_get_vstr (const u8 *block, u8 *buffer, int buflen)
{
	int length;

	BUG_ON (!block || !buffer);

	length = block[0];
	if (length >= buflen) {
		ldm_error ("Truncating string %d -> %d.", length, buflen);
		length = buflen - 1;
	}
	memcpy (buffer, block + 1, length);
	buffer[length] = 0;
	return length;
}
