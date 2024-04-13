static void vmsvga_invalidate_display(void *opaque)
{
    struct vmsvga_state_s *s = opaque;
    if (!s->enable) {
        s->vga.hw_ops->invalidate(&s->vga);
        return;
    }

    s->invalidated = 1;
}
