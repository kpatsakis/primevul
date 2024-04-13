int __glXDisp_GetFBConfigs(__GLXclientState *cl, GLbyte *pc)
{
    xGLXGetFBConfigsReq *req = (xGLXGetFBConfigsReq *) pc;
    return DoGetFBConfigs(cl, req->screen);
}
