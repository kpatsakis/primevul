XFixesUnionRegion (Display *dpy, XserverRegion dst,
		   XserverRegion src1, XserverRegion src2)
{
    XFixesExtDisplayInfo	*info = XFixesFindDisplay (dpy);
    xXFixesUnionRegionReq	*req;

    XFixesSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (XFixesUnionRegion, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesUnionRegion;
    req->source1 = src1;
    req->source2 = src2;
    req->destination = dst;
    UnlockDisplay (dpy);
    SyncHandle();
}
