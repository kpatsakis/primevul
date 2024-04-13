Status _xvmc_destroy_context (
    Display *dpy,
    XvMCContext *context
)
{
    XExtDisplayInfo *info = xvmc_find_display(dpy);
    xvmcDestroyContextReq  *req;

    XvMCCheckExtension (dpy, info, BadImplementation);

    LockDisplay (dpy);
    XvMCGetReq (DestroyContext, req);
    req->context_id = context->context_id;
    UnlockDisplay (dpy);
    SyncHandle ();
    return Success;
}
