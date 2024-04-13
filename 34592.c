ProcChangeWindowAttributes(ClientPtr client)
{
    WindowPtr pWin;

    REQUEST(xChangeWindowAttributesReq);
    int len, rc;
    Mask access_mode = 0;

    REQUEST_AT_LEAST_SIZE(xChangeWindowAttributesReq);
    access_mode |= (stuff->valueMask & CWEventMask) ? DixReceiveAccess : 0;
    access_mode |= (stuff->valueMask & ~CWEventMask) ? DixSetAttrAccess : 0;
    rc = dixLookupWindow(&pWin, stuff->window, client, access_mode);
    if (rc != Success)
        return rc;
    len = client->req_len - bytes_to_int32(sizeof(xChangeWindowAttributesReq));
    if (len != Ones(stuff->valueMask))
        return BadLength;
    return ChangeWindowAttributes(pWin,
                                  stuff->valueMask, (XID *) &stuff[1], client);
}
