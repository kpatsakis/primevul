XFixesSetRegion (Display *dpy, XserverRegion region,
		 XRectangle *rectangles, int nrectangles)
{
    XFixesExtDisplayInfo	*info = XFixesFindDisplay (dpy);
    xXFixesSetRegionReq		*req;
    long    			len;

    XFixesSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (XFixesSetRegion, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesSetRegion;
    req->region = region;
    len = ((long) nrectangles) << 1;
    SetReqLen (req, len, len);
    len <<= 2;
    Data16 (dpy, (short *) rectangles, len);
    UnlockDisplay (dpy);
    SyncHandle();
}
