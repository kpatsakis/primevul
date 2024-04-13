fbStore_r8g8b8 (FbBits *bits, const CARD32 *values, int x, int width, miIndexedPtr indexed)
{
    int i;
    CARD8 *pixel = ((CARD8 *) bits) + 3*x;
    for (i = 0; i < width; ++i) {
        Store24(pixel, READ(values + i));
        pixel += 3;
    }
}
