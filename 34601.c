ProcCreateColormap(ClientPtr client)
{
    VisualPtr pVisual;
    ColormapPtr pmap;
    Colormap mid;
    WindowPtr pWin;
    ScreenPtr pScreen;

    REQUEST(xCreateColormapReq);
    int i, result;

    REQUEST_SIZE_MATCH(xCreateColormapReq);

    if ((stuff->alloc != AllocNone) && (stuff->alloc != AllocAll)) {
        client->errorValue = stuff->alloc;
        return BadValue;
    }
    mid = stuff->mid;
    LEGAL_NEW_RESOURCE(mid, client);
    result = dixLookupWindow(&pWin, stuff->window, client, DixGetAttrAccess);
    if (result != Success)
        return result;

    pScreen = pWin->drawable.pScreen;
    for (i = 0, pVisual = pScreen->visuals;
         i < pScreen->numVisuals; i++, pVisual++) {
        if (pVisual->vid != stuff->visual)
            continue;
        return CreateColormap(mid, pScreen, pVisual, &pmap,
                              (int) stuff->alloc, client->index);
    }
    client->errorValue = stuff->visual;
    return BadMatch;
}
