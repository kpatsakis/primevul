int __glXDisp_CreateGLXPixmap(__GLXclientState *cl, GLbyte *pc)
{
    xGLXCreateGLXPixmapReq *req = (xGLXCreateGLXPixmapReq *) pc;
    __GLXconfig *config;
    __GLXscreen *pGlxScreen;
    int err;

    if (!validGlxScreen(cl->client, req->screen, &pGlxScreen, &err))
	return err;
    if (!validGlxVisual(cl->client, pGlxScreen, req->visual, &config, &err))
	return err;

    return DoCreateGLXPixmap(cl->client, pGlxScreen, config,
			     req->pixmap, req->glxpixmap);
}
