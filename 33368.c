fbFetchPixel_a8r8g8b8 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    return READ((CARD32 *)bits + offset);
}
