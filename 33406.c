fbFetch_x1r5g5b5 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    const CARD16 *pixel = (const CARD16 *)bits + x;
    const CARD16 *end = pixel + width;
    while (pixel < end) {
        CARD32  p = READ(pixel++);
        CARD32  r,g,b;

        r = ((p & 0x7c00) | ((p & 0x7000) >> 5)) << 9;
        g = ((p & 0x03e0) | ((p & 0x0380) >> 5)) << 6;
        b = ((p & 0x001c) | ((p & 0x001f) << 5)) >> 2;
        WRITE(buffer++, (0xff000000 | r | g | b));
    }
}
