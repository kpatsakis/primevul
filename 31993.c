XFixesExpandRegion (Display *dpy, XserverRegion dst, XserverRegion src,
		    unsigned left, unsigned right,
		    unsigned top, unsigned bottom)
{
    XFixesExtDisplayInfo	*info = XFixesFindDisplay (dpy);
    xXFixesExpandRegionReq	*req;

    XFixesSimpleCheckExtension (dpy, info);
    LockDisplay (dpy);
    GetReq (XFixesExpandRegion, req);
    req->reqType = info->codes->major_opcode;
    req->xfixesReqType = X_XFixesExpandRegion;
    req->source = src;
    req->destination = dst;
    req->left = left;
    req->right = right;
    req->top = top;
    req->bottom = bottom;
    UnlockDisplay (dpy);
    SyncHandle();
}
