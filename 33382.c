fbFetchPixel_x4a4 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    CARD32   pixel = READ((CARD8 *) bits + offset);

    return ((pixel & 0xf) | ((pixel & 0xf) << 4)) << 24;
}
