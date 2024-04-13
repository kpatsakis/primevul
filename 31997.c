XFixesRegionExtents (Display *dpy, XserverRegion dst, XserverRegion src)
{
    XFixesExtDisplayInfo	*info = XFixesFindDisplay (dpy);
    xXFixesRegionExtentsReq	*req;

    XFixesSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (XFixesRegionExtents, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesRegionExtents;
    req->source = src;
    req->destination = dst;
    UnlockDisplay (dpy);
    SyncHandle();
}
