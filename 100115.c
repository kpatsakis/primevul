virDomainMigrateGetCompressionCache(virDomainPtr domain,
                                    unsigned long long *cacheSize,
                                    unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "cacheSize=%p, flags=%x", cacheSize, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckNonNullArgGoto(cacheSize, error);

    if (conn->driver->domainMigrateGetCompressionCache) {
        if (conn->driver->domainMigrateGetCompressionCache(domain, cacheSize,
                                                           flags) < 0)
            goto error;
        return 0;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(conn);
    return -1;
}
