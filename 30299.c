setdevicecolor_cont(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    es_ptr ep = esp, pstage;
    int code = 0, stage, base;

    pstage = ep;
    base = (int)ep[-1].value.intval;
    stage = (int)pstage->value.intval;
    /* If we get a continuation from a sub-procedure, we will want to come back
     * here afterward, to do any remaining stages. We need to set up for that now.
     * so that our continuation is ahead of the sub-proc's continuation.
     */
    check_estack(1);
    /* May need to push a /Device... name on the stack so make sure we have space */
    check_ostack(1);
    /* The push_op_estack macro increments esp before use, so we don't need to */
    push_op_estack(setdevicecolor_cont);

    do {
        switch(stage) {
            case 0:
                make_int(pstage, ++stage);
                push(1);
                switch(base) {
                    case 0: /* DeviceGray */
                        code = name_enter_string(imemory, "DeviceGray", op);
                        break;
                    case 1: /* DeviceRGB */
                        code = name_enter_string(imemory, "DeviceRGB", op);
                        break;
                    case 2: /* DeviceCMYK */
                        code = name_enter_string(imemory, "DeviceCMYK", op);
                        break;
                }
                if (code < 0)
                    return code;
                code = zsetcolorspace(i_ctx_p);
                if (code != 0)
                    return code;
                break;
            case 1:
                make_int(pstage, ++stage);
                code = zsetcolor(i_ctx_p);
                if (code != 0)
                    return code;
                break;
            case 2:
                esp -= 3;
                return o_pop_estack;
                break;
        }
    }while(1);
    return 0;
}
