int __glXDisp_GetDrawableAttributesSGIX(__GLXclientState *cl, GLbyte *pc)
{
    xGLXGetDrawableAttributesSGIXReq *req =
	(xGLXGetDrawableAttributesSGIXReq *)pc;
    
    return DoGetDrawableAttributes(cl, req->drawable);
}
