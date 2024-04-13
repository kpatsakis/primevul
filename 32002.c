XFixesSubtractRegion (Display *dpy, XserverRegion dst,
		      XserverRegion src1, XserverRegion src2)
{
    XFixesExtDisplayInfo	*info = XFixesFindDisplay (dpy);
    xXFixesSubtractRegionReq	*req;

    XFixesSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (XFixesSubtractRegion, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesSubtractRegion;
    req->source1 = src1;
    req->source2 = src2;
    req->destination = dst;
    UnlockDisplay (dpy);
    SyncHandle();
}
