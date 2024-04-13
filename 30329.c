zcolor_reset_transfer(i_ctx_t *i_ctx_p)
{
    gx_set_effective_transfer(igs);
    return zcolor_remap_color(i_ctx_p);
}
