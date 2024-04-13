static int cirrus_post_load(void *opaque, int version_id)
{
    CirrusVGAState *s = opaque;

    s->vga.gr[0x00] = s->cirrus_shadow_gr0 & 0x0f;
    s->vga.gr[0x01] = s->cirrus_shadow_gr1 & 0x0f;

    cirrus_update_memory_access(s);
    /* force refresh */
    s->vga.graphic_mode = -1;
    cirrus_update_bank_ptr(s, 0);
    cirrus_update_bank_ptr(s, 1);
    return 0;
}
