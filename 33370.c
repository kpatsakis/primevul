fbFetchPixel_b2g3r3 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    CARD32   pixel = READ((CARD8 *) bits + offset);
    CARD32  r,g,b;

    b = (((pixel & 0xc0)     ) |
	 ((pixel & 0xc0) >> 2) |
	 ((pixel & 0xc0) >> 4) |
	 ((pixel & 0xc0) >> 6));
    g = ((pixel & 0x38) | ((pixel & 0x38) >> 3) | ((pixel & 0x30) << 2)) << 8;
    r = (((pixel & 0x07)     ) |
	 ((pixel & 0x07) << 3) |
	 ((pixel & 0x06) << 6)) << 16;
    return (0xff000000 | r | g | b);
}
