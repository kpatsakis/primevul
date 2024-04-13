int __glXDisp_DestroyGLXPbufferSGIX(__GLXclientState *cl, GLbyte *pc)
{
    xGLXDestroyGLXPbufferSGIXReq *req = (xGLXDestroyGLXPbufferSGIXReq *) pc;

    return DoDestroyDrawable(cl, req->pbuffer, GLX_DRAWABLE_PBUFFER);
}
