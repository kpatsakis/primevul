zcurrentcmykcolor(i_ctx_t * i_ctx_p)
{
    int code;

    code = zcurrentcolor(i_ctx_p);
    if (code < 0)
        return code;
    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(7);
    push_mark_estack(es_other, colour_cleanup);
    esp++;
    /* variable to hold stack depth for tint transform */
    make_int(&esp[0], 0);
    esp++;
    /* Store the 'base' type color wanted, in this case CMYK */
    make_int(&esp[0], 3);
    make_int(&esp[1], 1);
    /* Store the 'stage' of processing (initially 0) */
    make_int(&esp[2], 0);
    /* Store a pointer to the color space stored on the operand stack
     * as the stack may grow unpredictably making further access
     * to the space difficult
     */
    esp[3] = istate->colorspace[0].array;
    esp += 3; /* The push_op_estack macro increments esp before using it */
    /* Finally, the actual continuation routine */
    push_op_estack(currentbasecolor_cont);
    return o_push_estack;
}
