zsetgray(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;   /* required by "push" macro */
    float value;
    int code;

    /* Gather numeric operand value(s) */
    code = float_params(op, 1, &value);
    if (code < 0)
        return code;
    /* Clamp numeric operand range(s) */
    if (value < 0)
        value = 0;
    else if (value > 1)
        value = 1;
    code = make_floats(op, &value, 1);
    if (code < 0)
        return code;

    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(5);
    push_mark_estack(es_other, colour_cleanup);
    esp++;
    /* variable to hold base type (0 = gray) */
    make_int(esp, 0);
    esp++;
    /* Store the 'stage' of processing (initially 0) */
    make_int(esp, 0);
    /* Finally, the actual continuation routine */
    push_op_estack(setdevicecolor_cont);
    return o_push_estack;
}
