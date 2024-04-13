static void fbFetch(PicturePtr pict, int x, int y, int width, CARD32 *buffer, CARD32 *mask, CARD32 maskBits)
{
    FbBits *bits;
    FbStride stride;
    int bpp;
    int xoff, yoff;
    fetchProc fetch = fetchProcForPicture(pict);
    miIndexedPtr indexed = (miIndexedPtr) pict->pFormat->index.devPrivate;

    fbGetDrawable (pict->pDrawable, bits, stride, bpp, xoff, yoff);
    x += xoff;
    y += yoff;

    bits += y*stride;

    fetch(bits, x, width, buffer, indexed);
    fbFinishAccess (pict->pDrawable);
}
