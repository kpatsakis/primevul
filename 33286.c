fbCombineAddC (CARD32 *dest, CARD32 *src, CARD32 *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        CARD32 s = READ(src + i);
        CARD32 m = READ(mask + i);
        CARD32 d = READ(dest + i);

	fbCombineMaskValueC (&s, &m);

        FbByteAdd(d, s);
        WRITE(dest + i, d);
    }
}
