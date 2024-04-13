fbFetch_b8g8r8 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    const CARD8 *pixel = (const CARD8 *)bits + 3*x;
    const CARD8 *end = pixel + 3*width;
    while (pixel < end) {
        CARD32 b = 0xff000000;
#if IMAGE_BYTE_ORDER == MSBFirst
        b |= (READ(pixel++));
        b |= (READ(pixel++) << 8);
        b |= (READ(pixel++) << 16);
#else
        b |= (READ(pixel++) << 16);
        b |= (READ(pixel++) << 8);
        b |= (READ(pixel++));
#endif
    }
}
