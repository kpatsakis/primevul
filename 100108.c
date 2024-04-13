virDomainGetDiskErrors(virDomainPtr dom,
                       virDomainDiskErrorPtr errors,
                       unsigned int maxerrors,
                       unsigned int flags)
{
    VIR_DOMAIN_DEBUG(dom, "errors=%p, maxerrors=%u, flags=%x",
                     errors, maxerrors, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);

    if (maxerrors)
        virCheckNonNullArgGoto(errors, error);
    else
        virCheckNullArgGoto(errors, error);

    if (dom->conn->driver->domainGetDiskErrors) {
        int ret = dom->conn->driver->domainGetDiskErrors(dom, errors,
                                                         maxerrors, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
