ProcClearToBackground(ClientPtr client)
{
    REQUEST(xClearAreaReq);
    WindowPtr pWin;
    int rc;

    REQUEST_SIZE_MATCH(xClearAreaReq);
    rc = dixLookupWindow(&pWin, stuff->window, client, DixWriteAccess);
    if (rc != Success)
        return rc;
    if (pWin->drawable.class == InputOnly) {
        client->errorValue = stuff->window;
        return BadMatch;
    }
    if ((stuff->exposures != xTrue) && (stuff->exposures != xFalse)) {
        client->errorValue = stuff->exposures;
        return BadValue;
    }
    (*pWin->drawable.pScreen->ClearToBackground) (pWin, stuff->x, stuff->y,
                                                  stuff->width, stuff->height,
                                                  (Bool) stuff->exposures);
    return Success;
}
