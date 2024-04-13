virDomainRestore(virConnectPtr conn, const char *from)
{
    VIR_DEBUG("conn=%p, from=%s", conn, NULLSTR(from));

    virResetLastError();

    virCheckConnectReturn(conn, -1);
    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(from, error);

    if (conn->driver->domainRestore) {
        int ret;
        char *absolute_from;

        /* We must absolutize the file path as the restore is done out of process */
        if (virFileAbsPath(from, &absolute_from) < 0) {
            virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                           _("could not build absolute input file path"));
            goto error;
        }

        ret = conn->driver->domainRestore(conn, absolute_from);

        VIR_FREE(absolute_from);

        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return -1;
}
