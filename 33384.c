fbFetchPixel_x4r4g4b4 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    CARD32  pixel = READ((CARD16 *) bits + offset);
    CARD32  r,g,b;

    r = ((pixel & 0x0f00) | ((pixel & 0x0f00) >> 4)) << 12;
    g = ((pixel & 0x00f0) | ((pixel & 0x00f0) >> 4)) << 8;
    b = ((pixel & 0x000f) | ((pixel & 0x000f) << 4));
    return (0xff000000 | r | g | b);
}
