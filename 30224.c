static int convert_transform(i_ctx_t * i_ctx_p, ref *arr, ref *pproc)
{
    os_ptr op = osp;   /* required by "push" macro */
    int code;

    /* buildfunction returns an operand on the stack. In fact
     * it replaces the lowest operand, so make sure there is an
     * empty sacrificial one present.
     */
    push(1);
    /* Start by trying a type 4 function */
    code = buildfunction(i_ctx_p, arr, pproc, 4);

    if (code < 0)
        /* If that fails, try a type 0. We prefer a type 4
         * because a type 0 will require us to sample the
         * space, which is expensive
         */
        code = buildfunction(i_ctx_p, arr, pproc, 0);

    return code;
}
