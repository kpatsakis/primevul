fbCombineSrcC (CARD32 *dest, CARD32 *src, CARD32 *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
	CARD32 s = READ(src + i);
	CARD32 m = READ(mask + i);

	fbCombineMaskValueC (&s, &m);

	WRITE(dest, s);
    }
}
