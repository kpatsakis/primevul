fbCombineOutU (CARD32 *dest, const CARD32 *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32 s = READ(src + i);
        CARD32 a = Alpha(~READ(dest + i));
        FbByteMul(s, a);
        WRITE(dest + i, s);
    }
}
