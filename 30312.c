static int validate_spaces(i_ctx_t *i_ctx_p, ref *arr, int *depth)
{
    ref space, *sp = &space;
    int code = 0;
    PS_colour_space_t *obj;

    ref_assign(&space, arr);
    *depth = 0;
    do {
        code = get_space_object(i_ctx_p, sp, &obj);
        if (code < 0)
            return code;

        (*depth)++;
        if (!obj->validateproc)
            break;

        code = obj->validateproc(i_ctx_p, &sp);
        if (code < 0)
            return code;
    }while(sp);
    return 0;
}
