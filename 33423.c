fbStore_b2g3r3 (FbBits *bits, const CARD32 *values, int x, int width, miIndexedPtr indexed)
{
    int i;
    CARD8   *pixel = ((CARD8 *) bits) + x;
    for (i = 0; i < width; ++i) {
        Split(READ(values + i));
        WRITE(pixel++, ((b     ) & 0xe0) |
	      ((g >> 3) & 0x1c) |
	      ((r >> 6)       ));
    }
}
