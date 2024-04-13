int redisSupervisedSystemd(void) {
    const char *notify_socket = getenv("NOTIFY_SOCKET");
    int fd = 1;
    struct sockaddr_un su;
    struct iovec iov;
    struct msghdr hdr;
    int sendto_flags = 0;

    if (!notify_socket) {
        serverLog(LL_WARNING,
                "systemd supervision requested, but NOTIFY_SOCKET not found");
        return 0;
    }

    if ((strchr("@/", notify_socket[0])) == NULL || strlen(notify_socket) < 2) {
        return 0;
    }

    serverLog(LL_NOTICE, "supervised by systemd, will signal readiness");
    if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        serverLog(LL_WARNING,
                "Can't connect to systemd socket %s", notify_socket);
        return 0;
    }

    memset(&su, 0, sizeof(su));
    su.sun_family = AF_UNIX;
    strncpy (su.sun_path, notify_socket, sizeof(su.sun_path) -1);
    su.sun_path[sizeof(su.sun_path) - 1] = '\0';

    if (notify_socket[0] == '@')
        su.sun_path[0] = '\0';

    memset(&iov, 0, sizeof(iov));
    iov.iov_base = "READY=1";
    iov.iov_len = strlen("READY=1");

    memset(&hdr, 0, sizeof(hdr));
    hdr.msg_name = &su;
    hdr.msg_namelen = offsetof(struct sockaddr_un, sun_path) +
        strlen(notify_socket);
    hdr.msg_iov = &iov;
    hdr.msg_iovlen = 1;

    unsetenv("NOTIFY_SOCKET");
#ifdef HAVE_MSG_NOSIGNAL
    sendto_flags |= MSG_NOSIGNAL;
#endif
    if (sendmsg(fd, &hdr, sendto_flags) < 0) {
        serverLog(LL_WARNING, "Can't send notification to systemd");
        close(fd);
        return 0;
    }
    close(fd);
    return 1;
}
