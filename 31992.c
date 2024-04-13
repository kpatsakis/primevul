XFixesDestroyRegion (Display *dpy, XserverRegion region)
{
    XFixesExtDisplayInfo	*info = XFixesFindDisplay (dpy);
    xXFixesDestroyRegionReq	*req;

    XFixesSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (XFixesDestroyRegion, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesDestroyRegion;
    req->region = region;
    UnlockDisplay (dpy);
    SyncHandle();
}
