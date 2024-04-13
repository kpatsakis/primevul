void *vnc_server_fb_ptr(VncDisplay *vd, int x, int y)
{
    uint8_t *ptr;

    ptr  = (uint8_t *)pixman_image_get_data(vd->server);
    ptr += y * vnc_server_fb_stride(vd);
    ptr += x * VNC_SERVER_FB_BYTES;
    return ptr;
}
