XFixesCreateRegionFromWindow (Display *dpy, Window window, int kind)
{
    XFixesExtDisplayInfo		*info = XFixesFindDisplay (dpy);
    xXFixesCreateRegionFromWindowReq	*req;
    XserverRegion			region;

    XFixesCheckExtension (dpy, info, 0);
    LockDisplay (dpy);
    GetReq (XFixesCreateRegionFromWindow, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesCreateRegionFromWindow;
    region = req->region = XAllocID (dpy);
    req->window = window;
    req->kind = kind;
    UnlockDisplay (dpy);
    SyncHandle();
    return region;
}
