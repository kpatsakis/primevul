zsetcmykcolor(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;   /* required by "push" macro */
    int code, i;
    float values[4];

    /* Gather numeric operand value(s) (also checks type) */
    code = float_params(op, 4, (float *)&values);
    if (code < 0)
        return code;
    /* Clamp numeric operand range(s) */
    for (i = 0;i < 4; i++) {
        if (values[i] < 0)
            values[i] = 0;
        else if (values[i] > 1)
            values[i] = 1;
    }

    code = make_floats(&op[-3], (const float *)&values, 4);
    if (code < 0)
        return code;

    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(5);
    push_mark_estack(es_other, colour_cleanup);
    esp++;
    /* variable to hold base type (2 = CMYK) */
    make_int(esp, 2);
    esp++;
    /* Store the 'stage' of processing (initially 0) */
    make_int(esp, 0);
    /* Finally, the actual continuation routine */
    push_op_estack(setdevicecolor_cont);
    return o_push_estack;
}
