fbStore_a8r8g8b8 (FbBits *bits, const CARD32 *values, int x, int width, miIndexedPtr indexed)
{
    MEMCPY_WRAPPED(((CARD32 *)bits) + x, values, width*sizeof(CARD32));
}
