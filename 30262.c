static int indexedbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    int code;

    if (*stage == 0) {
        /* Usefully /Indexed can't be the base of any other space, so we know
         * the current space in the graphics state is this one.
         */
        gs_color_space *pcs;
        pcs = gs_currentcolorspace(igs);

        /* Update the counters */
        *stage = 1;
        *cont = 1;

        /* Indexed spaces can have *either* a procedure or a string for the
         * lookup.
         */
        if (pcs->params.indexed.use_proc) {
            es_ptr ep = ++esp;
            ref proc;

            /* We have a procedure, set up the continuation to run the
             * lookup procedure. (The index is already on the operand stack)
             */
            check_estack(1);
            code = array_get(imemory, space, 3, &proc);
            if (code < 0)
                return code;
            *ep = proc;	/* lookup proc */
            return o_push_estack;
        } else {
            int i, index;
            os_ptr op = osp;
            unsigned char *ptr = (unsigned char *)pcs->params.indexed.lookup.table.data;

            *stage = 0;
            /* We have a string, start by retrieving the index from the op stack */
            /* Make sure its an integer! */
            if (!r_has_type(op, t_integer))
                return_error (gs_error_typecheck);
            index = op->value.intval;
            /* And remove it from the stack. */
            pop(1);
            op = osp;

            /* Make sure we have enough space on the op stack to hold
             * one value for each component of the alternate space
             */
            push(pcs->params.indexed.n_comps);
            op -= pcs->params.indexed.n_comps - 1;

            /* Move along the lookup table, one byte for each component , the
             * number of times required to get to the lookup for this index
             */
            ptr += index * pcs->params.indexed.n_comps;

            /* For all the components of the alternate space, push the value
             * of the component on the stack. The value is given by the byte
             * from the lookup table divided by 255 to give a value between
             * 0 and 1.
             */
            for (i = 0; i < pcs->params.indexed.n_comps; i++, op++) {
                float rval = (*ptr++) / 255.0;
                make_real(op, rval);
            }
            return 0;
        }
    } else {
        *stage = 0;
        *cont = 1;
        return 0;
    }
}
