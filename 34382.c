int __glXDisp_CreateContextWithConfigSGIX(__GLXclientState *cl, GLbyte *pc)
{
    xGLXCreateContextWithConfigSGIXReq *req = 
	(xGLXCreateContextWithConfigSGIXReq *) pc;
    __GLXconfig *config;
    __GLXscreen *pGlxScreen;
    int err;

    if (!validGlxScreen(cl->client, req->screen, &pGlxScreen, &err))
	return err;
    if (!validGlxFBConfig(cl->client, pGlxScreen, req->fbconfig, &config, &err))
	return err;

    return DoCreateContext(cl, req->context, req->shareList,
			   config, pGlxScreen, req->isDirect);
}
