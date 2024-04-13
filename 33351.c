fbCombineSrcU (CARD32 *dest, const CARD32 *src, int width)
{
    MEMCPY_WRAPPED(dest, src, width*sizeof(CARD32));
}
