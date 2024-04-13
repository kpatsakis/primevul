determineTextureTarget(ClientPtr client, XID glxDrawableID,
		       CARD32 *attribs, CARD32 numAttribs)
{
    GLenum target = 0;
    GLenum format = 0;
    int i, err;
    __GLXdrawable *pGlxDraw;

    if (!validGlxDrawable(client, glxDrawableID, GLX_DRAWABLE_PIXMAP,
			  DixWriteAccess, &pGlxDraw, &err))
	/* We just added it in CreatePixmap, so we should never get here. */
	return;

    for (i = 0; i < numAttribs; i++) {
	if (attribs[2 * i] == GLX_TEXTURE_TARGET_EXT) {
	    switch (attribs[2 * i + 1]) {
	    case GLX_TEXTURE_2D_EXT:
		target = GL_TEXTURE_2D;
		break;
	    case GLX_TEXTURE_RECTANGLE_EXT:
		target = GL_TEXTURE_RECTANGLE_ARB;
		break;
	    }
	}

	if (attribs[2 * i] == GLX_TEXTURE_FORMAT_EXT)
		format = attribs[2 * i + 1];
    }
 
    if (!target) {
	int w = pGlxDraw->pDraw->width, h = pGlxDraw->pDraw->height;
	
	if (h & (h - 1) || w & (w - 1))
	    target = GL_TEXTURE_RECTANGLE_ARB;
	else
	    target = GL_TEXTURE_2D;
    }

    pGlxDraw->target = target;
    pGlxDraw->format = format;
}
