static inline void vmsvga_update_rect(struct vmsvga_state_s *s,
                                      int x, int y, int w, int h)
{
    DisplaySurface *surface = qemu_console_surface(s->vga.con);
    int line;
    int bypl;
    int width;
    int start;
    uint8_t *src;
    uint8_t *dst;

    if (!vmsvga_verify_rect(surface, __func__, x, y, w, h)) {
        /* go for a fullscreen update as fallback */
        x = 0;
        y = 0;
        w = surface_width(surface);
        h = surface_height(surface);
    }

    bypl = surface_stride(surface);
    width = surface_bytes_per_pixel(surface) * w;
    start = surface_bytes_per_pixel(surface) * x + bypl * y;
    src = s->vga.vram_ptr + start;
    dst = surface_data(surface) + start;

    for (line = h; line > 0; line--, src += bypl, dst += bypl) {
        memcpy(dst, src, width);
    }
    dpy_gfx_update(s->vga.con, x, y, w, h);
}
