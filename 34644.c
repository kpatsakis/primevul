ProcQueryTextExtents(ClientPtr client)
{
    xQueryTextExtentsReply reply;
    FontPtr pFont;
    ExtentInfoRec info;
    unsigned long length;
    int rc;

    REQUEST(xQueryTextExtentsReq);
    REQUEST_AT_LEAST_SIZE(xQueryTextExtentsReq);

    rc = dixLookupFontable(&pFont, stuff->fid, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    length = client->req_len - bytes_to_int32(sizeof(xQueryTextExtentsReq));
    length = length << 1;
    if (stuff->oddLength) {
        if (length == 0)
            return BadLength;
        length--;
    }
    if (!QueryTextExtents(pFont, length, (unsigned char *) &stuff[1], &info))
        return BadAlloc;
    reply = (xQueryTextExtentsReply) {
        .type = X_Reply,
        .drawDirection = info.drawDirection,
        .sequenceNumber = client->sequence,
        .length = 0,
        .fontAscent = info.fontAscent,
        .fontDescent = info.fontDescent,
        .overallAscent = info.overallAscent,
        .overallDescent = info.overallDescent,
        .overallWidth = info.overallWidth,
        .overallLeft = info.overallLeft,
        .overallRight = info.overallRight
    };
    WriteReplyToClient(client, sizeof(xQueryTextExtentsReply), &reply);
    return Success;
}
