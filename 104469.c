static void vnc_listen_websocket_read(void *opaque)
{
    vnc_listen_read(opaque, true);
}
