static void vnc_client_cache_addr(VncState *client)
{
    VncBasicInfo *bi = vnc_basic_info_get_from_remote_addr(client->csock);

    if (bi) {
        client->info = g_malloc0(sizeof(*client->info));
        client->info->base = bi;
    }
}
