ProcConfigureWindow(ClientPtr client)
{
    WindowPtr pWin;

    REQUEST(xConfigureWindowReq);
    int len, rc;

    REQUEST_AT_LEAST_SIZE(xConfigureWindowReq);
    rc = dixLookupWindow(&pWin, stuff->window, client,
                         DixManageAccess | DixSetAttrAccess);
    if (rc != Success)
        return rc;
    len = client->req_len - bytes_to_int32(sizeof(xConfigureWindowReq));
    if (Ones((Mask) stuff->mask) != len)
        return BadLength;
    return ConfigureWindow(pWin, (Mask) stuff->mask, (XID *) &stuff[1], client);
}
