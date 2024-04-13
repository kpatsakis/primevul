static int set_dev_space(i_ctx_t * i_ctx_p, int components)
{
    int code, stage = 1, cont = 0;
    switch(components) {
        case 1:
            code = setgrayspace(i_ctx_p, (ref *)0, &stage, &cont, 1);
            break;
        case 3:
            code = setrgbspace(i_ctx_p, (ref *)0, &stage, &cont, 1);
            break;
        case 4:
            code = setcmykspace(i_ctx_p, (ref *)0, &stage, &cont, 1);
            break;
        default:
            code = gs_note_error(gs_error_rangecheck);
            break;
    }
    return code;
}
