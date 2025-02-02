static int devicenbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr  op = osp;   /* required by "push" macro */
    int code, use, n_comp;
    ref narray;

    code = devicentransform(i_ctx_p, space, &use, stage, stack_depth);
    if (code !=  0)
        return code;
    if (!use) {
        *stage = 0;
        *cont = 0;
        code = array_get(imemory, space, 1, &narray);
        if (code < 0)
            return code;
        n_comp = r_size(&narray);
        pop(n_comp);
        op = osp;
        switch(base) {
        case 0:
            push(1);
            make_real(op, 0.0);
            break;
        case 1:
        case 2:
            push(3);
            make_real(&op[-2], 0.0);
            make_real(&op[-1], 0.0);
            make_real(op, 0.0);
            break;
        case 3:
            push(4);
            make_real(&op[-3], 0.0);
            make_real(&op[-2], 0.0);
            make_real(&op[-1], 0.0);
            make_real(op, 0.0);
            break;
        }
    } else {
        *stage = 0;
        *cont = 1;
    }
    return 0;
}
