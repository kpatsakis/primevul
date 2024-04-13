XFixesInvertRegion (Display *dpy, XserverRegion dst,
		    XRectangle *rect, XserverRegion src)
{
    XFixesExtDisplayInfo	*info = XFixesFindDisplay (dpy);
    xXFixesInvertRegionReq	*req;

    XFixesSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (XFixesInvertRegion, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesInvertRegion;
    req->x = rect->x;
    req->y = rect->y;
    req->width = rect->width;
    req->height = rect->height;
    req->source = src;
    req->destination = dst;
    UnlockDisplay (dpy);
    SyncHandle();
}
