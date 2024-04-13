int vnc_server_fb_stride(VncDisplay *vd)
{
    return pixman_image_get_stride(vd->server);
}
