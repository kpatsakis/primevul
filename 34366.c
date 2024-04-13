DoCreateGLXDrawable(ClientPtr client, __GLXscreen *pGlxScreen,
		    __GLXconfig *config, DrawablePtr pDraw, XID drawableId,
		    XID glxDrawableId, int type)
{
    __GLXdrawable *pGlxDraw;

    if (pGlxScreen->pScreen != pDraw->pScreen)
	return BadMatch;

    pGlxDraw = pGlxScreen->createDrawable(client, pGlxScreen, pDraw,
					  drawableId, type,
					  glxDrawableId, config);
    if (pGlxDraw == NULL)
	return BadAlloc;

    if (!AddResource(glxDrawableId, __glXDrawableRes, pGlxDraw)) {
	pGlxDraw->destroy (pGlxDraw);
	return BadAlloc;
    }

    /* Add the glx drawable under the XID of the underlying X drawable
     * too.  That way we'll get a callback in DrawableGone and can
     * clean up properly when the drawable is destroyed. */
    if (drawableId != glxDrawableId &&
	!AddResource(pDraw->id, __glXDrawableRes, pGlxDraw)) {
	pGlxDraw->destroy (pGlxDraw);
	return BadAlloc;
    }

    return Success;
}
