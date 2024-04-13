static void fbStore(PicturePtr pict, int x, int y, int width, CARD32 *buffer)
{
    FbBits *bits;
    FbStride stride;
    int bpp;
    int xoff, yoff;
    storeProc store = storeProcForPicture(pict);
    miIndexedPtr indexed = (miIndexedPtr) pict->pFormat->index.devPrivate;

    fbGetDrawable (pict->pDrawable, bits, stride, bpp, xoff, yoff);
    x += xoff;
    y += yoff;

    bits += y*stride;
    store(bits, buffer, x, width, indexed);
    fbFinishAccess (pict->pDrawable);
}
