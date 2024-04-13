fbFetch_b2g3r3 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    const CARD8 *pixel = (const CARD8 *)bits + x;
    const CARD8 *end = pixel + width;
    while (pixel < end) {
        CARD32  p = READ(pixel++);
        CARD32  r,g,b;

        b = (((p & 0xc0)     ) |
             ((p & 0xc0) >> 2) |
             ((p & 0xc0) >> 4) |
             ((p & 0xc0) >> 6));
        g = ((p & 0x38) | ((p & 0x38) >> 3) | ((p & 0x30) << 2)) << 8;
        r = (((p & 0x07)     ) |
             ((p & 0x07) << 3) |
             ((p & 0x06) << 6)) << 16;
        WRITE(buffer++, (0xff000000 | r | g | b));
    }
}
