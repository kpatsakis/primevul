XFixesSetWindowShapeRegion (Display *dpy, Window win, int shape_kind,
			    int x_off, int y_off, XserverRegion region)
{
    XFixesExtDisplayInfo	    *info = XFixesFindDisplay (dpy);
    xXFixesSetWindowShapeRegionReq  *req;

    XFixesSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (XFixesSetWindowShapeRegion, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesSetWindowShapeRegion;
    req->dest = win;
    req->destKind = shape_kind;
    req->xOff = x_off;
    req->yOff = y_off;
    req->region = region;
    UnlockDisplay (dpy);
    SyncHandle();
}
