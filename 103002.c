static void vga_draw_blank(VGACommonState *s, int full_update)
{
    DisplaySurface *surface = qemu_console_surface(s->con);
    int i, w;
    uint8_t *d;

    if (!full_update)
        return;
    if (s->last_scr_width <= 0 || s->last_scr_height <= 0)
        return;

    w = s->last_scr_width * surface_bytes_per_pixel(surface);
    d = surface_data(surface);
    for(i = 0; i < s->last_scr_height; i++) {
        memset(d, 0, w);
        d += surface_stride(surface);
    }
    dpy_gfx_update(s->con, 0, 0,
                   s->last_scr_width, s->last_scr_height);
}
