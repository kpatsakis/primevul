fbFetch_x4a4 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    const CARD8 *pixel = (const CARD8 *)bits + x;
    const CARD8 *end = pixel + width;
    while (pixel < end) {
	CARD8 p = READ(pixel++) & 0xf;
        WRITE(buffer++, (p | (p << 4)) << 24);
    }
}
