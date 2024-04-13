static int devicencompareproc(i_ctx_t *i_ctx_p, ref *space, ref *testspace)
{
    ref sname1, sname2;
    int code;

    code = array_get(imemory, space, 1, &sname1);
    if (code < 0)
        return 0;

    code = array_get(imemory, testspace, 1, &sname2);
    if (code < 0)
        return 0;

    if (!r_is_array(&sname1))
        return 0;
    if (!r_is_array(&sname2))
        return 0;

    if (!comparearrays(i_ctx_p, &sname1, &sname2))
        return 0;

    code = array_get(imemory, testspace, 2, &sname1);
    if (code < 0)
        return 0;
    code = array_get(imemory, testspace, 2, &sname2);
    if (code < 0)
        return 0;
    if (r_type(&sname1) != r_type(&sname2))
        return 0;

    if (r_is_array(&sname1)) {
        if (!comparearrays(i_ctx_p, &sname1, &sname2))
            return 0;
    } else {
        if (!r_has_type(&sname1, t_name))
            return 0;
        if (!name_eq(&sname1, &sname2))
            return 0;
    }
    code = array_get(imemory, space, 3, &sname1);
    if (code < 0)
        return 0;
    code = array_get(imemory, testspace, 3, &sname2);
    if (code < 0)
        return 0;
    return(comparearrays(i_ctx_p, &sname1, &sname2));
}
