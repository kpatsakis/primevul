fbCombineClear (CARD32 *dest, const CARD32 *src, int width)
{
    MEMSET_WRAPPED(dest, 0, width*sizeof(CARD32));
}
