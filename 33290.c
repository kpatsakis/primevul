fbCombineAtopReverseU (CARD32 *dest, const CARD32 *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32 s = READ(src + i);
        CARD32 d = READ(dest + i);
        CARD32 src_a = Alpha(s);
        CARD32 dest_ia = Alpha(~d);

        FbByteAddMul(s, dest_ia, d, src_a);
        WRITE(dest + i, s);
    }
}
