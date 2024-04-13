virDomainPMSuspendForDuration(virDomainPtr dom,
                              unsigned int target,
                              unsigned long long duration,
                              unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "target=%u duration=%llu flags=%x",
                     target, duration, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    conn = dom->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    if (conn->driver->domainPMSuspendForDuration) {
        int ret;
        ret = conn->driver->domainPMSuspendForDuration(dom, target,
                                                       duration, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return -1;
}
