fbFetchPixel_b5g6r5 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    CARD32  pixel = READ((CARD16 *) bits + offset);
    CARD32  r,g,b;

    b = ((pixel & 0xf800) | ((pixel & 0xe000) >> 5)) >> 8;
    g = ((pixel & 0x07e0) | ((pixel & 0x0600) >> 6)) << 5;
    r = ((pixel & 0x001c) | ((pixel & 0x001f) << 5)) << 14;
    return (0xff000000 | r | g | b);
}
