validGlxFBConfigForWindow(ClientPtr client, __GLXconfig *config,
			  DrawablePtr pDraw, int *err)
{
    ScreenPtr pScreen = pDraw->pScreen;
    VisualPtr pVisual = NULL;
    XID vid;
    int i;

    vid = wVisual((WindowPtr)pDraw);
    for (i = 0; i < pScreen->numVisuals; i++) {
	if (pScreen->visuals[i].vid == vid) {
	    pVisual = &pScreen->visuals[i];
	    break;
	}
    }

    /* FIXME: What exactly should we check here... */
    if (pVisual->class != glxConvertToXVisualType(config->visualType) ||
	!(config->drawableType & GLX_WINDOW_BIT)) {
	client->errorValue = pDraw->id;
	*err = BadMatch;
	return FALSE;
    }

    return TRUE;
}
