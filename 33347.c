fbCombineOverU (CARD32 *dest, const CARD32 *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32 s = READ(src + i);
        CARD32 d = READ(dest + i);
        CARD32 ia = Alpha(~s);

        FbByteMulAdd(d, ia, s);
        WRITE(dest + i, d);
    }
}
