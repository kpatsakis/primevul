static int comparearrays(i_ctx_t * i_ctx_p, ref *m1, ref *m2)
{
    int i, code;
    ref ref1, ref2;

    if (r_size(m1) != r_size(m2))
        return 0;

    for (i=0;i < r_size(m1);i++) {
        code = array_get(imemory, m1, i, &ref1);
        if (code < 0)
            return 0;
        code = array_get(imemory, m2, i, &ref2);
        if (code < 0)
            return 0;

        if (r_type(&ref1) != r_type(&ref2))
            return 0;

        code = r_type(&ref1);
        if (code < 0)
            return code;
        switch(r_type(&ref1)) {
            case t_null:
                break;
            case t_boolean:
                if (ref1.value.boolval != ref2.value.boolval)
                    return 0;
                break;
            case t_integer:
                if (ref1.value.intval != ref2.value.intval)
                    return 0;
                break;
            case t_real:
                if (ref1.value.realval != ref2.value.realval)
                    return 0;
                break;
            case t_name:
                if (!name_eq(&ref1, &ref2))
                    return 0;
                break;
            case t_string:
                if (r_size(&ref1) != r_size(&ref2))
                    return 0;
                if (strncmp((const char *)ref1.value.const_bytes, (const char *)ref2.value.const_bytes, r_size(&ref1)) != 0)
                    return 0;
                break;
            case t_array:
            case t_mixedarray:
            case t_shortarray:
                if (!comparearrays(i_ctx_p, &ref1, &ref2))
                    return 0;
                break;
            case t_oparray:
                break;
            case t_operator:
                if (ref1.value.opproc != ref2.value.opproc)
                    return 0;
                break;
            case t__invalid:
            case t_dictionary:
            case t_file:
            case t_unused_array_:
            case t_struct:
            case t_astruct:
            case t_fontID:
            case t_save:
            case t_mark:
            case t_device:
                return 0;
            default:
                /* Some high frequency operators are defined starting at t_next_index
                 * I think as long as the 'type' of each is the same, we are OK
                 */
                break;
        }
    }
    return 1;
}
