static void fbStoreExternalAlpha(PicturePtr pict, int x, int y, int width, CARD32 *buffer)
{
    FbBits *bits, *alpha_bits;
    FbStride stride, astride;
    int bpp, abpp;
    int xoff, yoff;
    int ax, ay;
    storeProc store;
    storeProc astore;
    miIndexedPtr indexed = (miIndexedPtr) pict->pFormat->index.devPrivate;
    miIndexedPtr aindexed;

    if (!pict->alphaMap) {
        fbStore(pict, x, y, width, buffer);
	return;
    }

    store = storeProcForPicture(pict);
    astore = storeProcForPicture(pict->alphaMap);
    aindexed = (miIndexedPtr) pict->alphaMap->pFormat->index.devPrivate;

    ax = x;
    ay = y;

    fbGetDrawable (pict->pDrawable, bits, stride, bpp, xoff, yoff);
    x += xoff;
    y += yoff;
    fbGetDrawable (pict->alphaMap->pDrawable, alpha_bits, astride, abpp, xoff, yoff);
    ax += xoff;
    ay += yoff;

    bits       += y*stride;
    alpha_bits += (ay - pict->alphaOrigin.y)*astride;


    store(bits, buffer, x, width, indexed);
    astore(alpha_bits, buffer, ax - pict->alphaOrigin.x, width, aindexed);

    fbFinishAccess (pict->alphaMap->pDrawable);
    fbFinishAccess (pict->pDrawable);
}
