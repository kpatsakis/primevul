int __glXDisp_ClientInfo(__GLXclientState *cl, GLbyte *pc)
{
    xGLXClientInfoReq *req = (xGLXClientInfoReq *) pc;
    const char *buf;
   
    cl->GLClientmajorVersion = req->major;
    cl->GLClientminorVersion = req->minor;
    free(cl->GLClientextensions);
    buf = (const char *)(req+1);
    cl->GLClientextensions = strdup(buf);

    return Success;
}
