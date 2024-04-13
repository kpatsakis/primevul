fbStore_b8g8r8 (FbBits *bits, const CARD32 *values, int x, int width, miIndexedPtr indexed)
{
    int i;
    CARD8 *pixel = ((CARD8 *) bits) + 3*x;
    for (i = 0; i < width; ++i) {
        CARD32 val = READ(values + i);
#if IMAGE_BYTE_ORDER == MSBFirst
        WRITE(pixel++, Blue(val));
        WRITE(pixel++, Green(val));
        WRITE(pixel++, Red(val));
#else
        WRITE(pixel++, Red(val));
        WRITE(pixel++, Green(val));
        WRITE(pixel++, Blue(val));
#endif
    }
}
