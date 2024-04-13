static int devicencolorants_cont(i_ctx_t *i_ctx_p)
{
    ref dict, *pdict = &dict, space[2], sname;
    int index, code, depth, stage;
    es_ptr ep = esp, pindex, pstage;
    os_ptr op = osp;
    gs_separation_name sep_name;

    pindex = &ep[-2];
    pstage = &ep[-1];
    index = (int)pindex->value.intval;
    stage = (int)pstage->value.intval;
    ref_assign(&dict, ep);

    do {
        index = dict_next(pdict, index, (ref *)&space);
        if (index == -1) {
            esp -= 4;
            return o_pop_estack;
        }

        if (stage == 0) {
            code = gs_gsave(igs);
            if (code < 0)
                return code;

            code = validate_spaces(i_ctx_p, &space[1], &depth);
            if (code < 0) {
                (void)gs_grestore(igs);
                return code;
            }

            /* If we get a continuation from a sub-procedure, we will want to come back
             * here afterward, to do any remaining stages. We need to set up for that now.
             * so that our continuation is ahead of the sub-proc's continuation.
             */
            check_estack(1);
            push(1);
            /* The push_op_estack macro increments esp before use, so we don't need to */
            push_op_estack(devicencolorants_cont);

            make_int(pstage, 1);
            *op = space[1];
            code = zsetcolorspace(i_ctx_p);
            if (code < 0) {
                (void)gs_grestore(igs);
                return code;
            } else
                return code;
        } else {
            stage = 0;

            switch (r_type(&space[0])) {
                case t_string:
                    code = name_from_string(imemory, &space[0], &sname);
                    if (code < 0){
                        (void)gs_grestore(igs);
                        return code;
                    }
                    sep_name = name_index(imemory, &sname);
                    break;
                case t_name:
                    sep_name = name_index(imemory, &space[0]);
                    break;
                default:
                    (void)gs_grestore(igs);
                    return_error(gs_error_typecheck);
                    break;
            }

            make_int(pindex, index);
            make_int(pstage, stage);
            gs_attachattributecolorspace(sep_name, igs);

            code = gs_grestore(igs);
            if (code < 0)
                return code;
        }
    }
    while(1);
}
