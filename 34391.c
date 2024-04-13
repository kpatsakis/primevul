int __glXDisp_DestroyGLXPixmap(__GLXclientState *cl, GLbyte *pc)
{
    xGLXDestroyGLXPixmapReq *req = (xGLXDestroyGLXPixmapReq *) pc;

    return DoDestroyDrawable(cl, req->glxpixmap, GLX_DRAWABLE_PIXMAP);
}
