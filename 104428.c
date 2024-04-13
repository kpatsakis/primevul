static VncBasicInfo *vnc_basic_info_get_from_remote_addr(int fd)
{
    struct sockaddr_storage sa;
    socklen_t salen;

    salen = sizeof(sa);
    if (getpeername(fd, (struct sockaddr*)&sa, &salen) < 0) {
        return NULL;
    }

    return vnc_basic_info_get(&sa, salen);
}
