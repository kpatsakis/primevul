fbStore_a1r1g1b1 (FbBits *bits, const CARD32 *values, int x, int width, miIndexedPtr indexed)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32  pixel;
        Splita(READ(values + i));
        pixel = (((a >> 4) & 0x8) |
                 ((r >> 5) & 0x4) |
                 ((g >> 6) & 0x2) |
                 ((b >> 7)      ));
        Store4(bits, i + x, pixel);
    }
}
