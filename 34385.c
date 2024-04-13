int __glXDisp_CreateGLXPixmapWithConfigSGIX(__GLXclientState *cl, GLbyte *pc)
{
    xGLXCreateGLXPixmapWithConfigSGIXReq *req = 
	(xGLXCreateGLXPixmapWithConfigSGIXReq *) pc;
    __GLXconfig *config;
    __GLXscreen *pGlxScreen;
    int err;

    if (!validGlxScreen(cl->client, req->screen, &pGlxScreen, &err))
	return err;
    if (!validGlxFBConfig(cl->client, pGlxScreen, req->fbconfig, &config, &err))
	return err;

    return DoCreateGLXPixmap(cl->client, pGlxScreen,
			     config, req->pixmap, req->glxpixmap);
}
