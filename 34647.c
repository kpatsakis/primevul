ProcSetClipRectangles(ClientPtr client)
{
    int nr, result;
    GC *pGC;

    REQUEST(xSetClipRectanglesReq);

    REQUEST_AT_LEAST_SIZE(xSetClipRectanglesReq);
    if ((stuff->ordering != Unsorted) && (stuff->ordering != YSorted) &&
        (stuff->ordering != YXSorted) && (stuff->ordering != YXBanded)) {
        client->errorValue = stuff->ordering;
        return BadValue;
    }
    result = dixLookupGC(&pGC, stuff->gc, client, DixSetAttrAccess);
    if (result != Success)
        return result;

    nr = (client->req_len << 2) - sizeof(xSetClipRectanglesReq);
    if (nr & 4)
        return BadLength;
    nr >>= 3;
    return SetClipRects(pGC, stuff->xOrigin, stuff->yOrigin,
                        nr, (xRectangle *) &stuff[1], (int) stuff->ordering);
}
