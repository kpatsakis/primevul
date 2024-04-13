int __glXDisp_ChangeDrawableAttributesSGIX(__GLXclientState *cl, GLbyte *pc)
{
    xGLXChangeDrawableAttributesSGIXReq *req =
	(xGLXChangeDrawableAttributesSGIXReq *)pc;

    return DoChangeDrawableAttributes(cl->client, req->drawable,
				      req->numAttribs, (CARD32 *) (req + 1));
}
