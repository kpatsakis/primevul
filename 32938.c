XvQueryExtension(
    Display *dpy,
    unsigned int *p_version,
    unsigned int *p_revision,
    unsigned int *p_requestBase,
    unsigned int *p_eventBase,
    unsigned int *p_errorBase)
{
    XExtDisplayInfo *info = xv_find_display(dpy);
    xvQueryExtensionReq *req;
    xvQueryExtensionReply rep;
    int status;

    XvCheckExtension(dpy, info, XvBadExtension);

    LockDisplay(dpy);

    XvGetReq(QueryExtension, req);

    if (!_XReply(dpy, (xReply *) &rep, 0, xFalse)) {
        status = XvBadExtension;
        goto out;
    }

    *p_version = rep.version;
    *p_revision = rep.revision;
    *p_requestBase = info->codes->major_opcode;
    *p_eventBase = info->codes->first_event;
    *p_errorBase = info->codes->first_error;

    status = Success;

  out:
    UnlockDisplay(dpy);
    SyncHandle();

    return status;
}
