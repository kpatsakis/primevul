ProcQueryFont(ClientPtr client)
{
    xQueryFontReply *reply;
    FontPtr pFont;
    int rc;

    REQUEST(xResourceReq);
    REQUEST_SIZE_MATCH(xResourceReq);

    rc = dixLookupFontable(&pFont, stuff->id, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    {
        xCharInfo *pmax = FONTINKMAX(pFont);
        xCharInfo *pmin = FONTINKMIN(pFont);
        int nprotoxcistructs;
        int rlength;

        nprotoxcistructs = (pmax->rightSideBearing == pmin->rightSideBearing &&
                            pmax->leftSideBearing == pmin->leftSideBearing &&
                            pmax->descent == pmin->descent &&
                            pmax->ascent == pmin->ascent &&
                            pmax->characterWidth == pmin->characterWidth) ?
            0 : N2dChars(pFont);

        rlength = sizeof(xQueryFontReply) +
            FONTINFONPROPS(FONTCHARSET(pFont)) * sizeof(xFontProp) +
            nprotoxcistructs * sizeof(xCharInfo);
        reply = calloc(1, rlength);
        if (!reply) {
            return BadAlloc;
        }

        reply->type = X_Reply;
        reply->length = bytes_to_int32(rlength - sizeof(xGenericReply));
        reply->sequenceNumber = client->sequence;
        QueryFont(pFont, reply, nprotoxcistructs);

        WriteReplyToClient(client, rlength, reply);
        free(reply);
        return Success;
    }
}
