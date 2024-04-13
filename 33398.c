fbFetch_b5g6r5 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    const CARD16 *pixel = (const CARD16 *)bits + x;
    const CARD16 *end = pixel + width;
    while (pixel < end) {
        CARD32  p = READ(pixel++);
        CARD32  r,g,b;

        b = ((p & 0xf800) | ((p & 0xe000) >> 5)) >> 8;
        g = ((p & 0x07e0) | ((p & 0x0600) >> 6)) << 5;
        r = ((p & 0x001c) | ((p & 0x001f) << 5)) << 14;
        WRITE(buffer++, (0xff000000 | r | g | b));
    }
}
