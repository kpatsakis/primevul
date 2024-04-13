__glXGetDrawable(__GLXcontext *glxc, GLXDrawable drawId, ClientPtr client,
		 int *error)
{
    DrawablePtr pDraw;
    __GLXdrawable *pGlxDraw;
    int rc;

    if (validGlxDrawable(client, drawId, GLX_DRAWABLE_ANY,
			 DixWriteAccess, &pGlxDraw, &rc)) {
	if (glxc != NULL && pGlxDraw->config != glxc->config) {
	    client->errorValue = drawId;
	    *error = BadMatch;
	    return NULL;
	}

	return pGlxDraw;
    }

    /* No active context and an unknown drawable, bail. */
    if (glxc == NULL) {
	    client->errorValue = drawId;
	    *error = BadMatch;
	    return NULL;
    }

    /* The drawId wasn't a GLX drawable.  Make sure it's a window and
     * create a GLXWindow for it.  Check that the drawable screen
     * matches the context screen and that the context fbconfig is
     * compatible with the window visual. */

    rc = dixLookupDrawable(&pDraw, drawId, client, 0, DixGetAttrAccess);
    if (rc != Success || pDraw->type != DRAWABLE_WINDOW) {
	client->errorValue = drawId;
	*error = __glXError(GLXBadDrawable);
	return NULL;
    }

    if (pDraw->pScreen != glxc->pGlxScreen->pScreen) {
	client->errorValue = pDraw->pScreen->myNum;
	*error = BadMatch;
	return NULL;
    }

    if (!validGlxFBConfigForWindow(client, glxc->config, pDraw, error))
	return NULL;

    pGlxDraw = glxc->pGlxScreen->createDrawable(client, glxc->pGlxScreen,
						pDraw, drawId,
						GLX_DRAWABLE_WINDOW,
						drawId, glxc->config);

    /* since we are creating the drawablePrivate, drawId should be new */
    if (!AddResource(drawId, __glXDrawableRes, pGlxDraw)) {
	pGlxDraw->destroy (pGlxDraw);
	*error = BadAlloc;
	return NULL;
    }

    return pGlxDraw;
}
