zcolor_remap_one_store(i_ctx_t *i_ctx_p, double min_value)
{
    int i;
    gx_transfer_map *pmap = r_ptr(esp, gx_transfer_map);

    if (ref_stack_count(&o_stack) < transfer_map_size)
        return_error(gs_error_stackunderflow);
    for (i = 0; i < transfer_map_size; i++) {
        double v;
        int code =
            real_param(ref_stack_index(&o_stack, transfer_map_size - 1 - i),
                       &v);

        if (code < 0)
            return code;
        pmap->values[i] =
            (v < min_value ? float2frac(min_value) :
             v >= 1.0 ? frac_1 :
             float2frac(v));
    }
    ref_stack_pop(&o_stack, transfer_map_size);
    esp--;			/* pop pointer to transfer map */
    return o_pop_estack;
}
