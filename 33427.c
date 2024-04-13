fbStore_c8 (FbBits *bits, const CARD32 *values, int x, int width, miIndexedPtr indexed)
{
    int i;
    CARD8   *pixel = ((CARD8 *) bits) + x;
    for (i = 0; i < width; ++i) {
        WRITE(pixel++, miIndexToEnt24(indexed,READ(values + i)));
    }
}
