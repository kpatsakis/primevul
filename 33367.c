fbFetchPixel_a8b8g8r8 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    CARD32  pixel = READ((CARD32 *)bits + offset);

    return ((pixel & 0xff000000) |
	    ((pixel >> 16) & 0xff) |
	    (pixel & 0x0000ff00) |
	    ((pixel & 0xff) << 16));
}
