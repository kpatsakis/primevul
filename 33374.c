fbFetchPixel_c8 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    CARD32   pixel = READ((CARD8 *) bits + offset);
    return indexed->rgba[pixel];
}
