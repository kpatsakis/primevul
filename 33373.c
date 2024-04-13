fbFetchPixel_c4 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    CARD32  pixel = Fetch4(bits, offset);

    return indexed->rgba[pixel];
}
