virConnectListDomains(virConnectPtr conn, int *ids, int maxids)
{
    VIR_DEBUG("conn=%p, ids=%p, maxids=%d", conn, ids, maxids);

    virResetLastError();

    virCheckConnectReturn(conn, -1);
    virCheckNonNullArgGoto(ids, error);
    virCheckNonNegativeArgGoto(maxids, error);

    if (conn->driver->connectListDomains) {
        int ret = conn->driver->connectListDomains(conn, ids, maxids);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();
 error:
    virDispatchError(conn);
    return -1;
}
