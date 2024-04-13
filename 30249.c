static int hasharray(i_ctx_t * i_ctx_p, ref *m1, gs_md5_state_t *md5)
{
    int i, code;
    ref ref1;

    for (i=0;i < r_size(m1);i++) {
        code = array_get(imemory, m1, i, &ref1);
        if (code < 0)
            return 0;

        code = r_type(&ref1);
        if (code < 0)
            return code;
        switch(r_type(&ref1)) {
            case t_null:
                break;
            case t_boolean:
                gs_md5_append(md5, (const gs_md5_byte_t *)&ref1.value.boolval, sizeof(ref1.value.boolval));
                break;
            case t_integer:
                gs_md5_append(md5, (const gs_md5_byte_t *)&ref1.value.intval, sizeof(ref1.value.intval));
                break;
            case t_real:
                gs_md5_append(md5, (const gs_md5_byte_t *)&ref1.value.realval, sizeof(ref1.value.realval));
                break;
            case t_name:
                gs_md5_append(md5, (const gs_md5_byte_t *)&ref1.value.pname, sizeof(ref1.value.pname));
                break;
            case t_string:
                gs_md5_append(md5, (const gs_md5_byte_t *)ref1.value.const_bytes, r_size(&ref1));
                break;
            case t_array:
            case t_mixedarray:
            case t_shortarray:
                if (!hasharray(i_ctx_p, &ref1, md5))
                    return 0;
                break;
            case t_oparray:
                break;
            case t_operator:
                gs_md5_append(md5, (const gs_md5_byte_t *)&ref1.value.opproc, sizeof(ref1.value.opproc));
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
