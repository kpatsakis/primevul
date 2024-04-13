XFixesTranslateRegion (Display *dpy, XserverRegion region, int dx, int dy)
{
    XFixesExtDisplayInfo	*info = XFixesFindDisplay (dpy);
    xXFixesTranslateRegionReq	*req;

    XFixesSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (XFixesTranslateRegion, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesTranslateRegion;
    req->region = region;
    req->dx = dx;
    req->dy = dy;
    UnlockDisplay (dpy);
    SyncHandle();
}
