ProcUngrabKey(ClientPtr client)
{
    REQUEST(xUngrabKeyReq);
    WindowPtr pWin;
    GrabPtr tempGrab;
    DeviceIntPtr keybd = PickKeyboard(client);
    int rc;

    REQUEST_SIZE_MATCH(xUngrabKeyReq);
    rc = dixLookupWindow(&pWin, stuff->grabWindow, client, DixGetAttrAccess);
    if (rc != Success)
        return rc;

    if (((stuff->key > keybd->key->xkbInfo->desc->max_key_code) ||
         (stuff->key < keybd->key->xkbInfo->desc->min_key_code))
        && (stuff->key != AnyKey)) {
        client->errorValue = stuff->key;
        return BadValue;
    }
    if ((stuff->modifiers != AnyModifier) &&
        (stuff->modifiers & ~AllModifiersMask)) {
        client->errorValue = stuff->modifiers;
        return BadValue;
    }
    tempGrab = AllocGrab(NULL);
    if (!tempGrab)
        return BadAlloc;
    tempGrab->resource = client->clientAsMask;
    tempGrab->device = keybd;
    tempGrab->window = pWin;
    tempGrab->modifiersDetail.exact = stuff->modifiers;
    tempGrab->modifiersDetail.pMask = NULL;
    tempGrab->modifierDevice = keybd;
    tempGrab->type = KeyPress;
    tempGrab->grabtype = CORE;
    tempGrab->detail.exact = stuff->key;
    tempGrab->detail.pMask = NULL;
    tempGrab->next = NULL;

    if (!DeletePassiveGrabFromList(tempGrab))
        rc = BadAlloc;

    FreeGrab(tempGrab);

    return rc;
}
