fbFetch_r3g3b2 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    const CARD8 *pixel = (const CARD8 *)bits + x;
    const CARD8 *end = pixel + width;
    while (pixel < end) {
        CARD32  p = READ(pixel++);
        CARD32  r,g,b;

        r = ((p & 0xe0) | ((p & 0xe0) >> 3) | ((p & 0xc0) >> 6)) << 16;
        g = ((p & 0x1c) | ((p & 0x18) >> 3) | ((p & 0x1c) << 3)) << 8;
        b = (((p & 0x03)     ) |
             ((p & 0x03) << 2) |
             ((p & 0x03) << 4) |
             ((p & 0x03) << 6));
        WRITE(buffer++, (0xff000000 | r | g | b));
    }
}
