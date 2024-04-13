DoQueryContext(__GLXclientState *cl, GLXContextID gcId)
{
    ClientPtr client = cl->client;
    __GLXcontext *ctx;
    xGLXQueryContextInfoEXTReply reply;
    int nProps;
    int *sendBuf, *pSendBuf;
    int nReplyBytes;
    int err;

    if (!validGlxContext(cl->client, gcId, DixReadAccess, &ctx, &err))
	return err;

    nProps = 3;
    reply.length = nProps << 1;
    reply.type = X_Reply;
    reply.sequenceNumber = client->sequence;
    reply.n = nProps;

    nReplyBytes = reply.length << 2;
    sendBuf = (int *)malloc((size_t)nReplyBytes);
    if (sendBuf == NULL) {
	return __glXError(GLXBadContext);	/* XXX: Is this correct? */
    }
    pSendBuf = sendBuf;
    *pSendBuf++ = GLX_SHARE_CONTEXT_EXT;
    *pSendBuf++ = (int)(ctx->share_id);
    *pSendBuf++ = GLX_VISUAL_ID_EXT;
    *pSendBuf++ = (int)(ctx->config->visualID);
    *pSendBuf++ = GLX_SCREEN_EXT;
    *pSendBuf++ = (int)(ctx->pGlxScreen->pScreen->myNum);

    if (client->swapped) {
	__glXSwapQueryContextInfoEXTReply(client, &reply, sendBuf);
    } else {
	WriteToClient(client, sz_xGLXQueryContextInfoEXTReply, (char *)&reply);
	WriteToClient(client, nReplyBytes, (char *)sendBuf);
    }
    free((char *)sendBuf);

    return Success;
}
