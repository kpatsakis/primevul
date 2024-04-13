fbStore_x8r8g8b8 (FbBits *bits, const CARD32 *values, int x, int width, miIndexedPtr indexed)
{
    int i;
    CARD32 *pixel = (CARD32 *)bits + x;
    for (i = 0; i < width; ++i)
        WRITE(pixel++, READ(values + i) & 0xffffff);
}
