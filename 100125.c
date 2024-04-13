virDomainSetMemoryStatsPeriod(virDomainPtr domain, int period,
                              unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "period=%d, flags=%x", period, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    /* This must be positive to set the balloon collection period */
    virCheckNonNegativeArgGoto(period, error);

    if (conn->driver->domainSetMemoryStatsPeriod) {
        int ret;
        ret = conn->driver->domainSetMemoryStatsPeriod(domain, period, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
