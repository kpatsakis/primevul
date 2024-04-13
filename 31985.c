Status _xvmc_create_surface (
    Display *dpy,
    XvMCContext *context,
    XvMCSurface *surface,
    int *priv_count,
    CARD32 **priv_data
)
{
    XExtDisplayInfo *info = xvmc_find_display(dpy);
    xvmcCreateSurfaceReply rep;
    xvmcCreateSurfaceReq  *req;

    *priv_count = 0;
    *priv_data = NULL;

    XvMCCheckExtension (dpy, info, BadImplementation);

    LockDisplay (dpy);
    XvMCGetReq (CreateSurface, req);

    surface->surface_id = XAllocID(dpy);
    surface->context_id = context->context_id;
    surface->surface_type_id = context->surface_type_id;
    surface->width = context->width;
    surface->height = context->height;

    req->surface_id = surface->surface_id;
    req->context_id = surface->context_id;
    if (!_XReply (dpy, (xReply *) &rep, 0, xFalse)) {
        UnlockDisplay (dpy);
        SyncHandle ();
        return BadImplementation;
    }

    if(rep.length) {
        if (rep.length < (INT_MAX >> 2))
            *priv_data = Xmalloc(rep.length << 2);
        if(*priv_data) {
            _XRead(dpy, (char*)(*priv_data), rep.length << 2);
            *priv_count = rep.length;
        } else
            _XEatDataWords(dpy, rep.length);
    }

    UnlockDisplay (dpy);
    SyncHandle ();
    return Success;
}
