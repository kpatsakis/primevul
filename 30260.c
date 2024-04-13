indexed_cont(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    es_ptr ep = esp;
    int i = (int)ep[csme_index].value.intval;

    if (i >= 0) {		/* i.e., not first time */
        int m = (int)ep[csme_num_components].value.intval;
        int code = float_params(op, m, &r_ptr(&ep[csme_map], gs_indexed_map)->values[i * m]);

        if (code < 0)
            return code;
        pop(m);
        op -= m;
        if (i == (int)ep[csme_hival].value.intval) {	/* All done. */
            esp -= num_csme;
            return o_pop_estack;
        }
    }
    push(1);
    ep[csme_index].value.intval = ++i;
    make_int(op, i);
    make_op_estack(ep + 1, indexed_cont);
    ep[2] = ep[csme_proc];	/* lookup proc */
    esp = ep + 2;
    return o_push_estack;
}
