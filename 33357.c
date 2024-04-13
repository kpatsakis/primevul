fbFetchPixel_a1 (const FbBits *bits, int offset, miIndexedPtr indexed)
{
    CARD32  pixel = ((CARD32 *)bits)[offset >> 5];
    CARD32  a;
#if BITMAP_BIT_ORDER == MSBFirst
    a = pixel >> (0x1f - (offset & 0x1f));
#else
    a = pixel >> (offset & 0x1f);
#endif
    a = a & 1;
    a |= a << 1;
    a |= a << 2;
    a |= a << 4;
    return a << 24;
}
