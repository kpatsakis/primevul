fbCombineOverC (CARD32 *dest, CARD32 *src, CARD32 *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
	CARD32 s = READ(src + i);
	CARD32 m = READ(mask + i);
	CARD32 a;

	fbCombineMaskC (&s, &m);

	a = ~m;
        if (a != 0xffffffff)
        {
            if (a)
            {
                CARD32 d = READ(dest + i);
                FbByteMulAddC(d, a, s);
                s = d;
            }
            WRITE(dest + i, s);
        }
    }
}
