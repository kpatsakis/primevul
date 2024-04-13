fbCombineInC (CARD32 *dest, CARD32 *src, CARD32 *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        CARD32 d = READ(dest + i);
        CARD16 a = d >> 24;
        CARD32 s = 0;
        if (a)
        {
	    CARD32 m = READ(mask + i);

	    s = READ(src + i);
	    fbCombineMaskValueC (&s, &m);
            if (a != 0xff)
            {
                FbByteMul(s, a);
            }
        }
        WRITE(dest + i, s);
    }
}
