fbCombineOutReverseC (CARD32 *dest, CARD32 *src, CARD32 *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
	CARD32 s = READ(src + i);
	CARD32 m = READ(mask + i);
	CARD32 a;

	fbCombineMaskAlphaC (&s, &m);

        a = ~m;
        if (a != 0xffffffff)
        {
            CARD32 d = 0;
            if (a)
            {
                d = READ(dest + i);
                FbByteMulC(d, a);
            }
            WRITE(dest + i, d);
        }
    }
}
