int __glXDisp_DestroyPixmap(__GLXclientState *cl, GLbyte *pc)
{
    xGLXDestroyPixmapReq *req = (xGLXDestroyPixmapReq *) pc;

    return DoDestroyDrawable(cl, req->glxpixmap, GLX_DRAWABLE_PIXMAP);
}
