virDomainGetMaxVcpus(virDomainPtr domain)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain);

    virResetLastError();

    virCheckDomainReturn(domain, -1);
    conn = domain->conn;

    if (conn->driver->domainGetMaxVcpus) {
        int ret;
        ret = conn->driver->domainGetMaxVcpus(domain);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
