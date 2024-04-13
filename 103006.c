static bool vga_endian_state_needed(void *opaque)
{
    VGACommonState *s = opaque;

    /*
     * Only send the endian state if it's different from the
     * default one, thus ensuring backward compatibility for
     * migration of the common case
     */
    return s->default_endian_fb != s->big_endian_fb;
}
