DoGetDrawableAttributes(__GLXclientState *cl, XID drawId)
{
    ClientPtr client = cl->client;
    xGLXGetDrawableAttributesReply reply;
    __GLXdrawable *pGlxDraw;
    CARD32 attributes[6];
    int numAttribs, error;

    if (!validGlxDrawable(client, drawId, GLX_DRAWABLE_ANY,
			  DixGetAttrAccess, &pGlxDraw, &error))
	return error;

    numAttribs = 3;
    reply.length = numAttribs << 1;
    reply.type = X_Reply;
    reply.sequenceNumber = client->sequence;
    reply.numAttribs = numAttribs;

    attributes[0] = GLX_TEXTURE_TARGET_EXT;
    attributes[1] = pGlxDraw->target == GL_TEXTURE_2D ? GLX_TEXTURE_2D_EXT :
	GLX_TEXTURE_RECTANGLE_EXT;
    attributes[2] = GLX_Y_INVERTED_EXT;
    attributes[3] = GL_FALSE;
    attributes[4] = GLX_EVENT_MASK;
    attributes[5] = pGlxDraw->eventMask;

    if (client->swapped) {
	__glXSwapGetDrawableAttributesReply(client, &reply, attributes);
    } else {
	WriteToClient(client, sz_xGLXGetDrawableAttributesReply,
		      (char *)&reply);
	WriteToClient(client, reply.length * sizeof (CARD32),
		      (char *)attributes);
    }

    return Success;
}
