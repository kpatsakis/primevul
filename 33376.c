fbFetchPixel_r1g2b1 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    CARD32  pixel = Fetch4(bits, offset);
    CARD32  r,g,b;

    r = ((pixel & 0x8) * 0xff) << 13;
    g = ((pixel & 0x6) * 0x55) << 7;
    b = ((pixel & 0x1) * 0xff);
    return 0xff000000|r|g|b;
}
