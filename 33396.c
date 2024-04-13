fbFetch_a8b8g8r8 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    const CARD32 *pixel = (CARD32 *)bits + x;
    const CARD32 *end = pixel + width;
    while (pixel < end) {
        WRITE(buffer++, ((READ(pixel) & 0xff00ff00) |
                         ((READ(pixel) >> 16) & 0xff) |
                         ((READ(pixel) & 0xff) << 16)));
        ++pixel;
    }
}
