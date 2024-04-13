ProcCreateGlyphCursor(ClientPtr client)
{
    CursorPtr pCursor;
    int res;

    REQUEST(xCreateGlyphCursorReq);

    REQUEST_SIZE_MATCH(xCreateGlyphCursorReq);
    LEGAL_NEW_RESOURCE(stuff->cid, client);

    res = AllocGlyphCursor(stuff->source, stuff->sourceChar,
                           stuff->mask, stuff->maskChar,
                           stuff->foreRed, stuff->foreGreen, stuff->foreBlue,
                           stuff->backRed, stuff->backGreen, stuff->backBlue,
                           &pCursor, client, stuff->cid);
    if (res != Success)
        return res;
    if (AddResource(stuff->cid, RT_CURSOR, (void *) pCursor))
        return Success;
    return BadAlloc;
}
