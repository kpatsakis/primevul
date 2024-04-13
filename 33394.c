fbFetch_a4 (const FbBits *bits, int x, int width, CARD32 *buffer, miIndexedPtr indexed)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32  p = Fetch4(bits, i + x);

        p |= p << 4;
        WRITE(buffer++, p << 24);
    }
}
