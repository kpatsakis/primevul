ivalidate_clean_spaces(i_ctx_t *i_ctx_p)
{
    if (gs_debug_c('?')) {
        ref_stack_cleanup(&d_stack);
        ref_stack_cleanup(&e_stack);
        ref_stack_cleanup(&o_stack);
        ivalidate_spaces();
    }
}
