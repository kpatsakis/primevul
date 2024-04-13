static int seticcspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    os_ptr op = osp;
    ref     ICCdict, *tempref, *altref=NULL, *nocie = NULL;
    int components, code;
    float range[8];

    code = dict_find_string(systemdict, "NOCIE", &nocie);
    if (code > 0) {
        if (!r_has_type(nocie, t_boolean))
            return_error(gs_error_typecheck);
    }
    *cont = 0;
    do {
        switch(*stage) {
            case 0:
                (*stage)++;
                code = array_get(imemory, r, 1, &ICCdict);
                if (code < 0)
                    return code;
                code = dict_find_string(&ICCdict, "N", &tempref);
                if (code < 0)
                    return code;
                if (code == 0)
                    return gs_note_error(gs_error_undefined);
                components = tempref->value.intval;
                if (components > count_of(range)/2)
                    return_error(gs_error_rangecheck);

                /* Don't allow ICCBased spaces if NOCIE is true */
                if (nocie && nocie->value.boolval) {
                    code = dict_find_string(&ICCdict, "Alternate", &altref); /* Alternate is optional */
                    if (code > 0 && (altref != NULL) && (r_type(altref) != t_null)) {
                        /* The PDF interpreter sets a null Alternate. If we have an
                         * Alternate, and its not null, and NOCIE is true, then use the
                         * Alternate instead of the ICC
                         */
                        push(1);
                        ref_assign(op, altref);
                        /* If CIESubst, we are already substituting for CIE, so use nosubst
                         * to prevent further substitution!
                         */
                        return setcolorspace_nosubst(i_ctx_p);
                    } else {
                        /* There's no /Alternate (or it is null), set a default space
                         * based on the number of components in the ICCBased space
                         */
                        code = set_dev_space(i_ctx_p, components);
                        if (code != 0)
                            return code;
                        *stage = 0;
                    }
                } else {
                    code = iccrange(i_ctx_p, r, (float *)&range);
                    if (code < 0)
                        return code;
                    code = dict_find_string(&ICCdict, "DataSource", &tempref);
                    if (code == 0)
                        return gs_note_error(gs_error_undefined);
                    /* Check for string based ICC and convert to a file */
                    if (r_has_type(tempref, t_string)){
                        uint n = r_size(tempref);
                        ref rss;

                        code = make_rss(i_ctx_p, &rss, tempref->value.const_bytes, n, r_space(tempref), 0L, n, false);
                        if (code < 0)
                            return code;
                        ref_assign(tempref, &rss);
                    }
                    /* Make space on operand stack to pass the ICC dictionary */
                    push(1);
                    ref_assign(op, &ICCdict);
                    code = seticc(i_ctx_p, components, op, (float *)&range);
                    if (code < 0) {
                        code = dict_find_string(&ICCdict, "Alternate", &altref); /* Alternate is optional */
                        if (code > 0 && (altref != NULL) && (r_type(altref) != t_null)) {
                            /* We have a /Alternate in the ICC space */
                            /* Our ICC dictionary still on operand stack, we can reuse the
                             * slot on the stack to hold the alternate space.
                             */
                            ref_assign(op, (ref *)altref);
                            /* If CIESubst, we are already substituting for CIE, so use nosubst
                             * to prevent further substitution!
                             */
                            if (CIESubst)
                                return setcolorspace_nosubst(i_ctx_p);
                            else
                                return zsetcolorspace(i_ctx_p);
                        } else {
                            /* We have no /Alternate in the ICC space, use hte /N key to
                             * determine an 'appropriate' default space.
                             */
                            code = set_dev_space(i_ctx_p, components);
                            if (code != 0)
                                return code;
                            *stage = 0;
                        }
                        pop(1);
                    }
                    if (code != 0)
                        return code;
                }
                break;
            case 1:
                /* All done, exit */
                *stage = 0;
                code = 0;
                break;
            default:
                return_error (gs_error_rangecheck);
                break;
        }
    }while(*stage);
    return code;
}
