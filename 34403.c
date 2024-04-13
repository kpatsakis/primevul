int __glXDisp_QueryContext(__GLXclientState *cl, GLbyte *pc)
{
    xGLXQueryContextReq *req = (xGLXQueryContextReq *) pc;

    return DoQueryContext(cl, req->context);
}
