Status XvMCQueryVersion (Display *dpy, int *major, int *minor)
{
    XExtDisplayInfo *info = xvmc_find_display(dpy);
    xvmcQueryVersionReply rep;
    xvmcQueryVersionReq  *req;

    XvMCCheckExtension (dpy, info, BadImplementation);

    LockDisplay (dpy);
    XvMCGetReq (QueryVersion, req);
    if (!_XReply (dpy, (xReply *) &rep, 0, xTrue)) {
        UnlockDisplay (dpy);
        SyncHandle ();
        return BadImplementation;
    }
    *major = rep.major;
    *minor = rep.minor;
    UnlockDisplay (dpy);
    SyncHandle ();
    return Success;
}
