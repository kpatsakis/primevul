fbFetchPixel_b1g2r1 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    CARD32  pixel = Fetch4(bits, offset);
    CARD32  r,g,b;

    b = ((pixel & 0x8) * 0xff) >> 3;
    g = ((pixel & 0x6) * 0x55) << 7;
    r = ((pixel & 0x1) * 0xff) << 16;
    return 0xff000000|r|g|b;
}
