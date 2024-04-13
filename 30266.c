static int labbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr op;
    int i, components=1;

    components = 3;
    pop(components);
    op = osp;
    components = 3;
    push(components);
    op -= components-1;
    for (i=0;i<components;i++) {
        make_real(op, (float)0);
        op++;
    }
    *stage = 0;
    *cont = 0;
    return 0;
}
