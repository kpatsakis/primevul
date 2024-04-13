fbCombineOutReverseU (CARD32 *dest, const CARD32 *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32 d = READ(dest + i);
        CARD32 a = Alpha(~READ(src + i));
        FbByteMul(d, a);
        WRITE(dest + i, d);
    }
}
