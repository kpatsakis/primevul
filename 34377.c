int __glXDisp_ChangeDrawableAttributes(__GLXclientState *cl, GLbyte *pc)
{
    xGLXChangeDrawableAttributesReq *req =
	(xGLXChangeDrawableAttributesReq *) pc;

    return DoChangeDrawableAttributes(cl->client, req->drawable,
				      req->numAttribs, (CARD32 *) (req + 1));
}
