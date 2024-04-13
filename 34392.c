int __glXDisp_DestroyPbuffer(__GLXclientState *cl, GLbyte *pc)
{
    xGLXDestroyPbufferReq *req = (xGLXDestroyPbufferReq *) pc;

    return DoDestroyDrawable(cl, req->pbuffer, GLX_DRAWABLE_PBUFFER);
}
