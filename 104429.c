static VncBasicInfo *vnc_basic_info_get_from_server_addr(int fd)
{
    struct sockaddr_storage sa;
    socklen_t salen;

    salen = sizeof(sa);
    if (getsockname(fd, (struct sockaddr*)&sa, &salen) < 0) {
        return NULL;
    }

    return vnc_basic_info_get(&sa, salen);
}
