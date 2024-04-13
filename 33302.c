fbCombineConjointInReverseC (CARD32 *dest, CARD32 *src, CARD32 *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineBIn);
}
