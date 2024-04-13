fbCombineMaskValueC (CARD32 *src, const CARD32 *mask)
{
    CARD32 a = READ(mask);
    CARD32	x;

    if (!a)
    {
	WRITE(src, 0);
	return;
    }

    if (a == 0xffffffff)
	return;

    x = READ(src);
    FbByteMulC(x, a);
    WRITE(src,x);
}
