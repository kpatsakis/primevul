fbStore_a4 (FbBits *bits, const CARD32 *values, int x, int width, miIndexedPtr indexed)
{
    int i;
    for (i = 0; i < width; ++i) {
        Store4(bits, i + x, READ(values + i)>>28);
    }
}
