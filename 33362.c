fbFetchPixel_a2b2g2r2 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    CARD32   pixel = READ((CARD8 *) bits + offset);
    CARD32   a,r,g,b;

    a = ((pixel & 0xc0) * 0x55) << 18;
    b = ((pixel & 0x30) * 0x55) >> 6;
    g = ((pixel & 0x0c) * 0x55) << 6;
    r = ((pixel & 0x03) * 0x55) << 16;
    return a|r|g|b;
}
