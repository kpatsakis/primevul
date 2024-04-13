fbFetch_x8r8g8b8 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    const CARD32 *pixel = (const CARD32 *)bits + x;
    const CARD32 *end = pixel + width;
    while (pixel < end) {
        WRITE(buffer++, READ(pixel++) | 0xff000000);
    }
}
