fbCombineMaskAlphaC (const CARD32 *src, CARD32 *mask)
{
    CARD32 a = READ(mask);
    CARD32	x;

    if (!a)
	return;

    x = READ(src) >> 24;
    if (x == 0xff)
	return;
    if (a == 0xffffffff)
    {
	x = x >> 24;
	x |= x << 8;
	x |= x << 16;
	WRITE(mask, x);
	return;
    }

    FbByteMul(a, x);
    WRITE(mask, a);
}
