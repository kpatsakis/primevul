virDomainGetEmulatorPinInfo(virDomainPtr domain, unsigned char *cpumap,
                            int maplen, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "cpumap=%p, maplen=%d, flags=%x",
                     cpumap, maplen, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);

    virCheckNonNullArgGoto(cpumap, error);
    virCheckPositiveArgGoto(maplen, error);

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_DOMAIN_AFFECT_LIVE,
                             VIR_DOMAIN_AFFECT_CONFIG,
                             error);

    conn = domain->conn;

    if (conn->driver->domainGetEmulatorPinInfo) {
        int ret;
        ret = conn->driver->domainGetEmulatorPinInfo(domain, cpumap,
                                                     maplen, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
