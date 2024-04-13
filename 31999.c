XFixesSetPictureClipRegion (Display *dpy, XID picture,
			    int clip_x_origin, int clip_y_origin,
			    XserverRegion region)
{
    XFixesExtDisplayInfo	    *info = XFixesFindDisplay (dpy);
    xXFixesSetPictureClipRegionReq  *req;

    XFixesSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (XFixesSetPictureClipRegion, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesSetPictureClipRegion;
    req->picture = picture;
    req->region = region;
    req->xOrigin = clip_x_origin;
    req->yOrigin = clip_y_origin;
    UnlockDisplay (dpy);
    SyncHandle();
}
