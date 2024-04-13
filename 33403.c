fbFetch_r1g2b1 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32  p = Fetch4(bits, i + x);
        CARD32  r,g,b;

        r = ((p & 0x8) * 0xff) << 13;
        g = ((p & 0x6) * 0x55) << 7;
        b = ((p & 0x1) * 0xff);
        WRITE(buffer++, 0xff000000|r|g|b);
    }
}
