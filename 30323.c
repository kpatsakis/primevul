static int validatepatternspace(i_ctx_t * i_ctx_p, ref **r)
{
    int code;
    ref tref;

    /* since makepattern has already been run, we don't need to do much validation */
    if (!r_has_type(*r, t_name)) {
        if (r_is_array(*r)) {
            if (r_size(*r) > 1) {
                code = array_get(imemory, *r, 1, &tref);
                if (code < 0)
                    return code;
                ref_assign(*r, &tref);
            } else
                *r = 0;
        } else
            return_error(gs_error_typecheck);
    } else
        *r = 0;
    return 0;
}
