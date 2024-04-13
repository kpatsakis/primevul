virDomainMigrateCheckNotLocal(const char *dconnuri)
{
    virURIPtr tempuri = NULL;
    int ret = -1;

    if (!(tempuri = virURIParse(dconnuri)))
        goto cleanup;
    if (!tempuri->server || STRPREFIX(tempuri->server, "localhost")) {
        virReportInvalidArg(dconnuri, "%s",
                            _("Attempt to migrate guest to the same host"));
        goto cleanup;
    }

    ret = 0;

 cleanup:
    virURIFree(tempuri);
    return ret;
}
