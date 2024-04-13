XFixesSetGCClipRegion (Display *dpy, GC gc,
		       int clip_x_origin, int clip_y_origin,
		       XserverRegion region)
{
    XFixesExtDisplayInfo	*info = XFixesFindDisplay (dpy);
    xXFixesSetGCClipRegionReq	    *req;

    XFixesSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (XFixesSetGCClipRegion, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesSetGCClipRegion;
    req->gc = gc->gid;
    req->region = region;
    req->xOrigin = clip_x_origin;
    req->yOrigin = clip_y_origin;
    UnlockDisplay (dpy);
    SyncHandle();
}
