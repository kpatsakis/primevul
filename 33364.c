fbFetchPixel_a4 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    CARD32  pixel = Fetch4(bits, offset);

    pixel |= pixel << 4;
    return pixel << 24;
}
