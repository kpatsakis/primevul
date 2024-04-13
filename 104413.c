VncInfo *qmp_query_vnc(Error **errp)
{
    VncInfo *info = g_malloc0(sizeof(*info));

    if (vnc_display == NULL || vnc_display->display == NULL) {
        info->enabled = false;
    } else {
        VncClientInfoList *cur_item = NULL;
        struct sockaddr_storage sa;
        socklen_t salen = sizeof(sa);
        char host[NI_MAXHOST];
        char serv[NI_MAXSERV];
        VncState *client;

        info->enabled = true;

        /* for compatibility with the original command */
        info->has_clients = true;

        QTAILQ_FOREACH(client, &vnc_display->clients, next) {
            VncClientInfoList *cinfo = g_malloc0(sizeof(*info));
            cinfo->value = qmp_query_vnc_client(client);

            /* XXX: waiting for the qapi to support GSList */
            if (!cur_item) {
                info->clients = cur_item = cinfo;
            } else {
                cur_item->next = cinfo;
                cur_item = cinfo;
            }
        }

        if (vnc_display->lsock == -1) {
            return info;
        }

        if (getsockname(vnc_display->lsock, (struct sockaddr *)&sa,
                        &salen) == -1) {
            error_set(errp, QERR_UNDEFINED_ERROR);
            goto out_error;
        }

        if (getnameinfo((struct sockaddr *)&sa, salen,
                        host, sizeof(host),
                        serv, sizeof(serv),
                        NI_NUMERICHOST | NI_NUMERICSERV) < 0) {
            error_set(errp, QERR_UNDEFINED_ERROR);
            goto out_error;
        }

        info->has_host = true;
        info->host = g_strdup(host);

        info->has_service = true;
        info->service = g_strdup(serv);

        info->has_family = true;
        info->family = inet_netfamily(sa.ss_family);

        info->has_auth = true;
        info->auth = g_strdup(vnc_auth_name(vnc_display));
    }

    return info;

out_error:
    qapi_free_VncInfo(info);
    return NULL;
}
