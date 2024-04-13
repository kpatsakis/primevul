fbCombineSaturateU (CARD32 *dest, const CARD32 *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32  s = READ(src + i);
        CARD32 d = READ(dest + i);
        CARD16  sa, da;

        sa = s >> 24;
        da = ~d >> 24;
        if (sa > da)
        {
            sa = FbIntDiv(da, sa);
            FbByteMul(s, sa);
        }
        FbByteAdd(d, s);
        WRITE(dest + i, d);
    }
}
