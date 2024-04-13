fbStore_r3g3b2 (FbBits *bits, const CARD32 *values, int x, int width, miIndexedPtr indexed)
{
    int i;
    CARD8   *pixel = ((CARD8 *) bits) + x;
    for (i = 0; i < width; ++i) {
        Split(READ(values + i));
        WRITE(pixel++, ((r     ) & 0xe0) |
	      ((g >> 3) & 0x1c) |
	      ((b >> 6)       ));
    }
}
