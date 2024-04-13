int __glXDisp_CreateGLXPbufferSGIX(__GLXclientState *cl, GLbyte *pc)
{
    xGLXCreateGLXPbufferSGIXReq *req = (xGLXCreateGLXPbufferSGIXReq *) pc;

    return DoCreatePbuffer(cl->client, req->screen, req->fbconfig,
			   req->width, req->height, req->pbuffer);
}
