ProcQueryTree(ClientPtr client)
{
    xQueryTreeReply reply;
    int rc, numChildren = 0;
    WindowPtr pChild, pWin, pHead;
    Window *childIDs = (Window *) NULL;

    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    rc = dixLookupWindow(&pWin, stuff->id, client, DixListAccess);
    if (rc != Success)
        return rc;

    reply = (xQueryTreeReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .root = pWin->drawable.pScreen->root->drawable.id,
        .parent = (pWin->parent) ? pWin->parent->drawable.id : (Window) None
    };
    pHead = RealChildHead(pWin);
    for (pChild = pWin->lastChild; pChild != pHead; pChild = pChild->prevSib)
        numChildren++;
    if (numChildren) {
        int curChild = 0;

        childIDs = malloc(numChildren * sizeof(Window));
        if (!childIDs)
            return BadAlloc;
        for (pChild = pWin->lastChild; pChild != pHead;
             pChild = pChild->prevSib)
            childIDs[curChild++] = pChild->drawable.id;
    }

    reply.nChildren = numChildren;
    reply.length = bytes_to_int32(numChildren * sizeof(Window));

    WriteReplyToClient(client, sizeof(xQueryTreeReply), &reply);
    if (numChildren) {
        client->pSwapReplyFunc = (ReplySwapPtr) Swap32Write;
        WriteSwappedDataToClient(client, numChildren * sizeof(Window),
                                 childIDs);
        free(childIDs);
    }

    return Success;
}
