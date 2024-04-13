static void vnc_listen_regular_read(void *opaque)
{
    vnc_listen_read(opaque, false);
}
