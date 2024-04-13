int __glXDisp_CreatePixmap(__GLXclientState *cl, GLbyte *pc)
{
    xGLXCreatePixmapReq *req = (xGLXCreatePixmapReq *) pc;
    __GLXconfig *config;
    __GLXscreen *pGlxScreen;
    int err;

    if (!validGlxScreen(cl->client, req->screen, &pGlxScreen, &err))
	return err;
    if (!validGlxFBConfig(cl->client, pGlxScreen, req->fbconfig, &config, &err))
	return err;

    err = DoCreateGLXPixmap(cl->client, pGlxScreen, config,
			    req->pixmap, req->glxpixmap);
    if (err != Success)
	return err;

    determineTextureTarget(cl->client, req->glxpixmap,
			   (CARD32*) (req + 1), req->numAttribs);

    return Success;
}
