int __glXDisp_QueryExtensionsString(__GLXclientState *cl, GLbyte *pc)
{
    ClientPtr client = cl->client;
    xGLXQueryExtensionsStringReq *req = (xGLXQueryExtensionsStringReq *) pc;
    xGLXQueryExtensionsStringReply reply;
    __GLXscreen *pGlxScreen;
    size_t n, length;
    char *buf;
    int err;

    if (!validGlxScreen(client, req->screen, &pGlxScreen, &err))
	return err;

    n = strlen(pGlxScreen->GLXextensions) + 1;
    length = __GLX_PAD(n) >> 2;
    reply.type = X_Reply;
    reply.sequenceNumber = client->sequence;
    reply.length = length;
    reply.n = n;

    /* Allocate buffer to make sure it's a multiple of 4 bytes big.*/
    buf = (char *) malloc(length << 2);
    if (buf == NULL)
        return BadAlloc;
    memcpy(buf, pGlxScreen->GLXextensions, n);

    if (client->swapped) {
        glxSwapQueryExtensionsStringReply(client, &reply, buf);
    } else {
        WriteToClient(client, sz_xGLXQueryExtensionsStringReply,(char *)&reply);
        WriteToClient(client, (int)(length << 2), (char *)buf);
    }

    free(buf);
    return Success;
}
