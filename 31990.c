XFixesCreateRegionFromGC (Display *dpy, GC gc)
{
    XFixesExtDisplayInfo		*info = XFixesFindDisplay (dpy);
    xXFixesCreateRegionFromGCReq	*req;
    XserverRegion			region;

    XFixesCheckExtension (dpy, info, 0);
    LockDisplay (dpy);
    GetReq (XFixesCreateRegionFromGC, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesCreateRegionFromGC;
    region = req->region = XAllocID (dpy);
    req->gc = gc->gid;
    UnlockDisplay (dpy);
    SyncHandle();
    return region;
}
