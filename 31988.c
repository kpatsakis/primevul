Status _xvmc_destroy_surface (
    Display *dpy,
    XvMCSurface *surface
)
{
    XExtDisplayInfo *info = xvmc_find_display(dpy);
    xvmcDestroySurfaceReq  *req;

    XvMCCheckExtension (dpy, info, BadImplementation);

    LockDisplay (dpy);
    XvMCGetReq (DestroySurface, req);
    req->surface_id = surface->surface_id;
    UnlockDisplay (dpy);
    SyncHandle ();
    return Success;
}
