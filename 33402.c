fbFetch_g1 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32  p = ((CARD32 *)bits)[(i+x) >> 5];
        CARD32 a;
#if BITMAP_BIT_ORDER == MSBFirst
        a = p >> (0x1f - ((i+x) & 0x1f));
#else
        a = p >> ((i+x) & 0x1f);
#endif
        a = a & 1;
        WRITE(buffer++, indexed->rgba[a]);
    }
}
