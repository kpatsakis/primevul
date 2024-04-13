fbStore_r1g2b1 (FbBits *bits, const CARD32 *values, int x, int width, miIndexedPtr indexed)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32  pixel;

        Split(READ(values + i));
        pixel = (((r >> 4) & 0x8) |
                 ((g >> 5) & 0x6) |
                 ((b >> 7)      ));
        Store4(bits, i + x, pixel);
    }
}
