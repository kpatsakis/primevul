int __glXDisp_IsDirect(__GLXclientState *cl, GLbyte *pc)
{
    ClientPtr client = cl->client;
    xGLXIsDirectReq *req = (xGLXIsDirectReq *) pc;
    xGLXIsDirectReply reply;
    __GLXcontext *glxc;
    int err;

    if (!validGlxContext(cl->client, req->context, DixReadAccess, &glxc, &err))
	return err;

    reply.isDirect = glxc->isDirect;
    reply.length = 0;
    reply.type = X_Reply;
    reply.sequenceNumber = client->sequence;

    if (client->swapped) {
	__glXSwapIsDirectReply(client, &reply);
    } else {
	WriteToClient(client, sz_xGLXIsDirectReply, (char *)&reply);
    }

    return Success;
}
