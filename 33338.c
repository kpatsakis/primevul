fbCombineMaskU (CARD32 *src, const CARD32 *mask, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32 a = READ(mask + i) >> 24;
        CARD32 s = READ(src + i);
        FbByteMul(s, a);
        WRITE(src + i, s);
    }
}
