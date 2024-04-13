virDomainLookupByID(virConnectPtr conn, int id)
{
    VIR_DEBUG("conn=%p, id=%d", conn, id);

    virResetLastError();

    virCheckConnectReturn(conn, NULL);
    virCheckNonNegativeArgGoto(id, error);

    if (conn->driver->domainLookupByID) {
        virDomainPtr ret;
        ret = conn->driver->domainLookupByID(conn, id);
        if (!ret)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(conn);
    return NULL;
}
