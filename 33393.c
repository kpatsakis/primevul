fbFetch_a2r2g2b2 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    const CARD8 *pixel = (const CARD8 *)bits + x;
    const CARD8 *end = pixel + width;
    while (pixel < end) {
        CARD32  p = READ(pixel++);
        CARD32   a,r,g,b;

        a = ((p & 0xc0) * 0x55) << 18;
        r = ((p & 0x30) * 0x55) << 12;
        g = ((p & 0x0c) * 0x55) << 6;
        b = ((p & 0x03) * 0x55);
        WRITE(buffer++, a|r|g|b);
    }
}
