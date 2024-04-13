fbStore_r5g6b5 (FbBits *bits, const CARD32 *values, int x, int width, miIndexedPtr indexed)
{
    int i;
    CARD16 *pixel = ((CARD16 *) bits) + x;
    for (i = 0; i < width; ++i) {
        CARD32 s = READ(values + i);
        WRITE(pixel++, ((s >> 3) & 0x001f) |
	      ((s >> 5) & 0x07e0) |
	      ((s >> 8) & 0xf800));
    }
}
