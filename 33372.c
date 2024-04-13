fbFetchPixel_b8g8r8 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    CARD8   *pixel = ((CARD8 *) bits) + (offset*3);
#if IMAGE_BYTE_ORDER == MSBFirst
    return (0xff000000 |
	    (READ(pixel + 2) << 16) |
	    (READ(pixel + 1) << 8) |
	    (READ(pixel + 0)));
#else
    return (0xff000000 |
	    (READ(pixel + 0) << 16) |
	    (READ(pixel + 1) << 8) |
	    (READ(pixel + 2)));
#endif
}
