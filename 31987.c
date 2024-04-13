Status _xvmc_destroy_subpicture(
    Display *dpy,
    XvMCSubpicture *subpicture
)
{
    XExtDisplayInfo *info = xvmc_find_display(dpy);
    xvmcDestroySubpictureReq  *req;

    XvMCCheckExtension (dpy, info, BadImplementation);

    LockDisplay (dpy);
    XvMCGetReq (DestroySubpicture, req);
    req->subpicture_id = subpicture->subpicture_id;
    UnlockDisplay (dpy);
    SyncHandle ();
    return Success;
}
