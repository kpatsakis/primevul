ProcQueryPointer(ClientPtr client)
{
    xQueryPointerReply rep;
    WindowPtr pWin, t;
    DeviceIntPtr mouse = PickPointer(client);
    DeviceIntPtr keyboard;
    SpritePtr pSprite;
    int rc;

    REQUEST(xResourceReq);
    REQUEST_SIZE_MATCH(xResourceReq);

    rc = dixLookupWindow(&pWin, stuff->id, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;
    rc = XaceHook(XACE_DEVICE_ACCESS, client, mouse, DixReadAccess);
    if (rc != Success && rc != BadAccess)
        return rc;

    keyboard = GetMaster(mouse, MASTER_KEYBOARD);

    pSprite = mouse->spriteInfo->sprite;
    if (mouse->valuator->motionHintWindow)
        MaybeStopHint(mouse, client);
    rep = (xQueryPointerReply) {
        .type = X_Reply,
        .sequenceNumber = client->sequence,
        .length = 0,
        .mask = event_get_corestate(mouse, keyboard),
        .root = (GetCurrentRootWindow(mouse))->drawable.id,
        .rootX = pSprite->hot.x,
        .rootY = pSprite->hot.y,
        .child = None
    };
    if (pSprite->hot.pScreen == pWin->drawable.pScreen) {
        rep.sameScreen = xTrue;
        rep.winX = pSprite->hot.x - pWin->drawable.x;
        rep.winY = pSprite->hot.y - pWin->drawable.y;
        for (t = pSprite->win; t; t = t->parent)
            if (t->parent == pWin) {
                rep.child = t->drawable.id;
                break;
            }
    }
    else {
        rep.sameScreen = xFalse;
        rep.winX = 0;
        rep.winY = 0;
    }

#ifdef PANORAMIX
    if (!noPanoramiXExtension) {
        rep.rootX += screenInfo.screens[0]->x;
        rep.rootY += screenInfo.screens[0]->y;
        if (stuff->id == rep.root) {
            rep.winX += screenInfo.screens[0]->x;
            rep.winY += screenInfo.screens[0]->y;
        }
    }
#endif

    if (rc == BadAccess) {
        rep.mask = 0;
        rep.child = None;
        rep.rootX = 0;
        rep.rootY = 0;
        rep.winX = 0;
        rep.winY = 0;
    }

    WriteReplyToClient(client, sizeof(xQueryPointerReply), &rep);

    return Success;
}
