fbCombineDisjointAtopReverseC (CARD32 *dest, CARD32 *src, CARD32 *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineBAtop);
}
