fbFetch_a1r1g1b1 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32  p = Fetch4(bits, i + x);
        CARD32  a,r,g,b;

        a = ((p & 0x8) * 0xff) << 21;
        r = ((p & 0x4) * 0xff) << 14;
        g = ((p & 0x2) * 0xff) << 7;
        b = ((p & 0x1) * 0xff);
        WRITE(buffer++, a|r|g|b);
    }
}
