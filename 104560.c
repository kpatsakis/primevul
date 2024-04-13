XSetModifierMapping(
    register Display *dpy,
    register XModifierKeymap *modifier_map)
{
    register xSetModifierMappingReq *req;
    xSetModifierMappingReply rep;
    int         mapSize = modifier_map->max_keypermod << 3;	/* 8 modifiers */

    LockDisplay(dpy);
    GetReq(SetModifierMapping, req);
    req->length += mapSize >> 2;
    req->numKeyPerModifier = modifier_map->max_keypermod;

    Data(dpy, modifier_map->modifiermap, mapSize);

    (void) _XReply(dpy, (xReply *) & rep,
	(SIZEOF(xSetModifierMappingReply) - SIZEOF(xReply)) >> 2, xTrue);
    UnlockDisplay(dpy);
    SyncHandle();
    return (rep.success);
}
