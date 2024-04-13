fbCombineOverReverseC (CARD32 *dest, CARD32 *src, CARD32 *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        CARD32 d = READ(dest + i);
        CARD32 a = ~d >> 24;

        if (a)
        {
            CARD32 s = READ(src + i);
	    CARD32 m = READ(mask + i);

	    fbCombineMaskValueC (&s, &m);

            if (a != 0xff)
            {
                FbByteMulAdd(s, a, d);
            }
            WRITE(dest + i, s);
        }
    }
}
