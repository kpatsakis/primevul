virDomainMigratePrepare(virConnectPtr dconn,
                        char **cookie,
                        int *cookielen,
                        const char *uri_in,
                        char **uri_out,
                        unsigned long flags,
                        const char *dname,
                        unsigned long bandwidth)
{
    VIR_DEBUG("dconn=%p, cookie=%p, cookielen=%p, uri_in=%s, uri_out=%p, "
              "flags=%lx, dname=%s, bandwidth=%lu", dconn, cookie, cookielen,
              NULLSTR(uri_in), uri_out, flags, NULLSTR(dname), bandwidth);

    virResetLastError();

    virCheckConnectReturn(dconn, -1);
    virCheckReadOnlyGoto(dconn->flags, error);

    if (dconn->driver->domainMigratePrepare) {
        int ret;
        ret = dconn->driver->domainMigratePrepare(dconn, cookie, cookielen,
                                                  uri_in, uri_out,
                                                  flags, dname, bandwidth);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dconn);
    return -1;
}
