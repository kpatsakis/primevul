static VncServerInfo *vnc_server_info_get(void)
{
    VncServerInfo *info;
    VncBasicInfo *bi = vnc_basic_info_get_from_server_addr(vnc_display->lsock);
    if (!bi) {
        return NULL;
    }

    info = g_malloc(sizeof(*info));
    info->base = bi;
    info->has_auth = true;
    info->auth = g_strdup(vnc_auth_name(vnc_display));
    return info;
}
