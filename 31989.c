XFixesCopyRegion (Display *dpy, XserverRegion dst, XserverRegion src)
{
    XFixesExtDisplayInfo	*info = XFixesFindDisplay (dpy);
    xXFixesCopyRegionReq	*req;

    XFixesSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (XFixesCopyRegion, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesCopyRegion;
    req->source = src;
    req->destination = dst;
    UnlockDisplay (dpy);
    SyncHandle();
}
