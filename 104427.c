static VncBasicInfo *vnc_basic_info_get(struct sockaddr_storage *sa,
                                        socklen_t salen)
{
    VncBasicInfo *info;
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];
    int err;

    if ((err = getnameinfo((struct sockaddr *)sa, salen,
                           host, sizeof(host),
                           serv, sizeof(serv),
                           NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
        VNC_DEBUG("Cannot resolve address %d: %s\n",
                  err, gai_strerror(err));
        return NULL;
    }

    info = g_malloc0(sizeof(VncBasicInfo));
    info->host = g_strdup(host);
    info->service = g_strdup(serv);
    info->family = inet_netfamily(sa->ss_family);
    return info;
}
