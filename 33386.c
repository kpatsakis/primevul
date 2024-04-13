static void fbFetchSolid(PicturePtr pict, int x, int y, int width, CARD32 *buffer, CARD32 *mask, CARD32 maskBits)
{
    FbBits *bits;
    FbStride stride;
    int bpp;
    int xoff, yoff;
    CARD32 color;
    CARD32 *end;
    fetchPixelProc fetch = fetchPixelProcForPicture(pict);
    miIndexedPtr indexed = (miIndexedPtr) pict->pFormat->index.devPrivate;

    fbGetDrawable (pict->pDrawable, bits, stride, bpp, xoff, yoff);
    bits += yoff*stride + (xoff*bpp >> FB_SHIFT);

    color = fetch(bits, 0, indexed);

    end = buffer + width;
    while (buffer < end)
        WRITE(buffer++, color);
    fbFinishAccess (pict->pDrawable);
}
