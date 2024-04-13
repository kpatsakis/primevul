int __glXDisp_ReleaseTexImageEXT(__GLXclientState *cl, GLbyte *pc)
{
    xGLXVendorPrivateReq *req = (xGLXVendorPrivateReq *) pc;
    ClientPtr		 client = cl->client;
    __GLXdrawable	*pGlxDraw;
    __GLXcontext	*context;
    GLXDrawable		 drawId;
    int			 buffer;
    int			 error;

    pc += __GLX_VENDPRIV_HDR_SIZE;

    drawId = *((CARD32 *) (pc));
    buffer = *((INT32 *)  (pc + 4));
    
    context = __glXForceCurrent (cl, req->contextTag, &error);
    if (!context)
	return error;

    if (!validGlxDrawable(client, drawId, GLX_DRAWABLE_PIXMAP,
			  DixReadAccess, &pGlxDraw, &error))
	return error;

    if (!context->textureFromPixmap)
	return __glXError(GLXUnsupportedPrivateRequest);

    return context->textureFromPixmap->releaseTexImage(context,
						       buffer,
						       pGlxDraw);
}
