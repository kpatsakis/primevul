virDomainGetBlockInfo(virDomainPtr domain, const char *disk,
                      virDomainBlockInfoPtr info, unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(domain, "info=%p, flags=%x", info, flags);

    virResetLastError();

    if (info)
        memset(info, 0, sizeof(*info));

    virCheckDomainReturn(domain, -1);
    virCheckNonEmptyStringArgGoto(disk, error);
    virCheckNonNullArgGoto(info, error);

    conn = domain->conn;

    if (conn->driver->domainGetBlockInfo) {
        int ret;
        ret = conn->driver->domainGetBlockInfo(domain, disk, info, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(domain->conn);
    return -1;
}
