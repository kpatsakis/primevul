fbStore_c4 (FbBits *bits, const CARD32 *values, int x, int width, miIndexedPtr indexed)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32  pixel;

        pixel = miIndexToEnt24(indexed, READ(values + i));
        Store4(bits, i + x, pixel);
    }
}
