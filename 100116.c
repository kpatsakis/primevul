virDomainMigratePerform3(virDomainPtr domain,
                         const char *xmlin,
                         const char *cookiein,
                         int cookieinlen,
                         char **cookieout,
                         int *cookieoutlen,
                         const char *dconnuri,
                         const char *uri,
                         unsigned long flags,
                         const char *dname,
                         unsigned long bandwidth)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "xmlin=%s cookiein=%p, cookieinlen=%d, "
                     "cookieout=%p, cookieoutlen=%p, dconnuri=%s, "
                     "uri=%s, flags=%lx, dname=%s, bandwidth=%lu",
                     NULLSTR(xmlin), cookiein, cookieinlen,
                     cookieout, cookieoutlen, NULLSTR(dconnuri),
                     NULLSTR(uri), flags, NULLSTR(dname), bandwidth);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainMigratePerform3) {
        int ret;
        ret = conn->driver->domainMigratePerform3(domain, xmlin,
                                                  cookiein, cookieinlen,
                                                  cookieout, cookieoutlen,
                                                  dconnuri, uri,
                                                  flags, dname, bandwidth);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
