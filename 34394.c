int __glXDisp_DestroyWindow(__GLXclientState *cl, GLbyte *pc)
{
    xGLXDestroyWindowReq *req = (xGLXDestroyWindowReq *) pc;

    return DoDestroyDrawable(cl, req->glxwindow, GLX_DRAWABLE_WINDOW);
}
