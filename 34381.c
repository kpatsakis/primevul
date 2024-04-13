int __glXDisp_CreateContext(__GLXclientState *cl, GLbyte *pc)
{
    xGLXCreateContextReq *req = (xGLXCreateContextReq *) pc;
    __GLXconfig *config;
    __GLXscreen *pGlxScreen;
    int err;

    if (!validGlxScreen(cl->client, req->screen, &pGlxScreen, &err))
	return err;
    if (!validGlxVisual(cl->client, pGlxScreen, req->visual, &config, &err))
	return err;

    return DoCreateContext(cl, req->context, req->shareList,
			   config, pGlxScreen, req->isDirect);
}
