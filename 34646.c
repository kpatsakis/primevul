ProcReparentWindow(ClientPtr client)
{
    WindowPtr pWin, pParent;

    REQUEST(xReparentWindowReq);
    int rc;

    REQUEST_SIZE_MATCH(xReparentWindowReq);
    rc = dixLookupWindow(&pWin, stuff->window, client, DixManageAccess);
    if (rc != Success)
        return rc;
    rc = dixLookupWindow(&pParent, stuff->parent, client, DixAddAccess);
    if (rc != Success)
        return rc;
    if (!SAME_SCREENS(pWin->drawable, pParent->drawable))
        return BadMatch;
    if ((pWin->backgroundState == ParentRelative) &&
        (pParent->drawable.depth != pWin->drawable.depth))
        return BadMatch;
    if ((pWin->drawable.class != InputOnly) &&
        (pParent->drawable.class == InputOnly))
        return BadMatch;
    return ReparentWindow(pWin, pParent,
                          (short) stuff->x, (short) stuff->y, client);
}
