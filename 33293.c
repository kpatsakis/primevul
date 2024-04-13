fbCombineClearC (CARD32 *dest, CARD32 *src, CARD32 *mask, int width)
{
    MEMSET_WRAPPED(dest, 0, width*sizeof(CARD32));
}
