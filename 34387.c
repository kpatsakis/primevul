int __glXDisp_CreatePbuffer(__GLXclientState *cl, GLbyte *pc)
{
    xGLXCreatePbufferReq	*req = (xGLXCreatePbufferReq *) pc;
    CARD32			*attrs;
    int				 width, height, i;

    attrs = (CARD32 *) (req + 1);
    width = 0;
    height = 0;

    for (i = 0; i < req->numAttribs; i++) {
	switch (attrs[i * 2]) {
	case GLX_PBUFFER_WIDTH:
	    width = attrs[i * 2 + 1];
	    break;
	case GLX_PBUFFER_HEIGHT:
	    height = attrs[i * 2 + 1];
	    break;
	case GLX_LARGEST_PBUFFER:
	case GLX_PRESERVED_CONTENTS:
	    /* FIXME: huh... */
	    break;
	}
    }

    return DoCreatePbuffer(cl->client, req->screen, req->fbconfig,
			   width, height, req->pbuffer);
}
