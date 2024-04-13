ReformatImage(char *base, int nbytes, int bpp, int order)
{
    switch (bpp) {
    case 1:                    /* yuck */
        if (BITMAP_BIT_ORDER != order)
            BitOrderInvert((unsigned char *) base, nbytes);
#if IMAGE_BYTE_ORDER != BITMAP_BIT_ORDER && BITMAP_SCANLINE_UNIT != 8
        ReformatImage(base, nbytes, BITMAP_SCANLINE_UNIT, order);
#endif
        break;
    case 4:
        break;                  /* yuck */
    case 8:
        break;
    case 16:
        if (IMAGE_BYTE_ORDER != order)
            TwoByteSwap((unsigned char *) base, nbytes);
        break;
    case 32:
        if (IMAGE_BYTE_ORDER != order)
            FourByteSwap((unsigned char *) base, nbytes);
        break;
    }
}
