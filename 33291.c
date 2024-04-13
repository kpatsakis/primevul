fbCombineAtopU (CARD32 *dest, const CARD32 *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32 s = READ(src + i);
        CARD32 d = READ(dest + i);
        CARD32 dest_a = Alpha(d);
        CARD32 src_ia = Alpha(~s);

        FbByteAddMul(s, dest_a, d, src_ia);
        WRITE(dest + i, s);
    }
}
