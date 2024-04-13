static u64 ldm_get_vnum (const u8 *block)
{
	u64 tmp = 0;
	u8 length;

	BUG_ON (!block);

	length = *block++;

	if (length && length <= 8)
		while (length--)
			tmp = (tmp << 8) | *block++;
	else
		ldm_error ("Illegal length %d.", length);

	return tmp;
}
