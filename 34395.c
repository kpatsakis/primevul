int __glXDisp_GetDrawableAttributes(__GLXclientState *cl, GLbyte *pc)
{
    xGLXGetDrawableAttributesReq *req = (xGLXGetDrawableAttributesReq *)pc;

    return DoGetDrawableAttributes(cl, req->drawable);
}
