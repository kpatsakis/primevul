virDomainMigratePrepare3(virConnectPtr dconn,
                         const char *cookiein,
                         int cookieinlen,
                         char **cookieout,
                         int *cookieoutlen,
                         const char *uri_in,
                         char **uri_out,
                         unsigned long flags,
                         const char *dname,
                         unsigned long bandwidth,
                         const char *dom_xml)
{
    VIR_DEBUG("dconn=%p, cookiein=%p, cookieinlen=%d, cookieout=%p, "
              "cookieoutlen=%p, uri_in=%s, uri_out=%p, flags=%lx, dname=%s, "
              "bandwidth=%lu, dom_xml=%s",
              dconn, cookiein, cookieinlen, cookieout, cookieoutlen, NULLSTR(uri_in),
              uri_out, flags, NULLSTR(dname), bandwidth, NULLSTR(dom_xml));

    virResetLastError();

    virCheckConnectReturn(dconn, -1);
    virCheckReadOnlyGoto(dconn->flags, error);

    if (dconn->driver->domainMigratePrepare3) {
        int ret;
        ret = dconn->driver->domainMigratePrepare3(dconn,
                                                   cookiein, cookieinlen,
                                                   cookieout, cookieoutlen,
                                                   uri_in, uri_out,
                                                   flags, dname, bandwidth,
                                                   dom_xml);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dconn);
    return -1;
}
