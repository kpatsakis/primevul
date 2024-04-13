static inline void vmsvga_check_size(struct vmsvga_state_s *s)
{
    DisplaySurface *surface = qemu_console_surface(s->vga.con);

    if (s->new_width != surface_width(surface) ||
        s->new_height != surface_height(surface) ||
        s->new_depth != surface_bits_per_pixel(surface)) {
        int stride = (s->new_depth * s->new_width) / 8;
        pixman_format_code_t format =
            qemu_default_pixman_format(s->new_depth, true);
        trace_vmware_setmode(s->new_width, s->new_height, s->new_depth);
        surface = qemu_create_displaysurface_from(s->new_width, s->new_height,
                                                  format, stride,
                                                  s->vga.vram_ptr);
        dpy_gfx_replace_surface(s->vga.con, surface);
        s->invalidated = 1;
    }
}
