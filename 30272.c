static int patternbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr op;
    int i, components=0;

    if (r_size(space) > 1) {
        const gs_color_space *  pcs = gs_currentcolorspace(igs);
        const gs_client_color * pcc = gs_currentcolor(igs);
        int                     n = cs_num_components(pcs);
        bool                    push_pattern = n < 0;
        gs_pattern_instance_t * pinst = pcc->pattern;

        if (pinst != 0 && pattern_instance_uses_base_space(pinst)) {
            /* check for pattern */
            if (push_pattern)
                pop(1);	    /* The pattern instance */
            *stage = 0;
            *cont = 1;
            return 0;
        }
        /* If the pattern isn't yet initialised, or doesn't use the
         * base space, treat as uncolored and return defaults below
         * Fall Through.
         */
    }

    pop(1);
    op = osp;
    switch(base) {
        case 0:
            components = 1;
            break;
        case 1:
        case 2:
            components = 3;
            break;
        case 3:
            components = 4;
            break;
    }
    push(components);
    op -= components-1;
    for (i=0;i<components;i++) {
        make_real(op, (float)0);
        op++;
    }
    if (components == 4) {
        op--;
        make_real(op, (float)1);
    }
    *stage = 0;
    *cont = 0;
    return 0;
}
