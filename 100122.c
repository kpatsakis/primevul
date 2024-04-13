virDomainPinVcpuFlags(virDomainPtr domain, unsigned int vcpu,
                      unsigned char *cpumap, int maplen, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "vcpu=%u, cpumap=%p, maplen=%d, flags=%x",
                     vcpu, cpumap, maplen, flags);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    virCheckReadOnlyGoto(conn->flags, error);
    virCheckNonNullArgGoto(cpumap, error);
    virCheckPositiveArgGoto(maplen, error);

    if (conn->driver->domainPinVcpuFlags) {
        int ret;
        ret = conn->driver->domainPinVcpuFlags(domain, vcpu, cpumap, maplen, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
